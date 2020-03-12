/*
 * ASTFunction.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "function.h"

#include <algorithm>
#include <ext/alloc_traits.h>
#include <iostream>
#include <llvm/ADT/Twine.h>
#include <llvm/ADT/iterator_range.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <stack>
#include <stddef.h>
#include <unordered_map>

#include "../bc/build_context.h"
#include "../type/function_type.h"
#include "../type/type_name.h"
#include "block_expression.h"
#include "expression.h"
#include "peglib.h"
#include "variable_declaration.h"

namespace llvm {
class Type;
} // namespace llvm

namespace by::ast {

ASTFunction::ASTFunction(const std::shared_ptr<peg::Ast> &ast)
    : ASTExpression(ast, nullptr) {
  size_t varscount = ast->nodes.size() - 2;
  name = std::to_string(ast->nodes[0]);
  type::TypeName_ptr returntype;
  if (const std::shared_ptr<peg::Ast> &astreturntype = *(ast->nodes.end() - 2);
      astreturntype->original_name == "TypeName") {
    returntype = std::make_shared<const type::TypeName>(astreturntype);
    --varscount;
  } else {
    returntype = type::TypeName::None;
  }

  for (size_t i = 0; i < varscount; ++i) {
    parameters.push_back(
        std::make_shared<ASTVariableDeclaration>(ast->nodes[1 + i], parent));
  }

  type::FunctionType func_type(*returntype);
  for (auto &para : parameters) {
    func_type.parameters.emplace_back(type::TypeName(*para->get_type()));
  }
  type = std::make_shared<const type::FunctionType>(func_type);

  blockexpression = std::make_shared<ASTBlockExpression>(
      parameters, ast->nodes.back(), parent);
}

auto ASTFunction::determine_type(type::variable_map &symbol)
    -> type::TypeName_ptr {
  if (!type) {
    type::FunctionType_ptr blocktype =
        std::make_shared<const type::FunctionType>(
            *blockexpression->determine_type(symbol),
            static_cast<const type::FunctionType>(*type).parameters);
    type = blocktype;
    symbol.emplace(name, type);
  } else {
    symbol.emplace(name, type);
    type::TypeName_ptr blocktype = blockexpression->determine_type(symbol);
    if (*blocktype != *type) {
      throw by::type::type_deduction_exeption(ast, type,
                                              blockexpression->get_type());
    }
  }
  return type;
}

void ASTFunction::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  for (const auto &parameter : parameters) {
    parameter->get_dependencies(functions, types);
  }
  if (*type) {
    types.insert(std::to_string(*type));
  }
  blockexpression->get_dependencies(functions, types);
}

auto ASTFunction::get_name() const -> std::string { return name; }

auto ASTFunction::build_ir(std::unique_ptr<by::bc::BuildContext> &bc) const
    -> llvm::Value * {
  bc->ast_stack.push(this);

  llvm::FunctionType *function_type =
      std::static_pointer_cast<const type::FunctionType>(type)
          ->get_llvm_function_type(bc->context);
  if (name == "main") {
    std::vector<llvm::Type *> llvm_parameters;
    llvm_parameters.emplace_back(llvm::Type::getInt32Ty(bc->context));
    llvm_parameters.emplace_back(
        llvm::Type::getInt8PtrTy(bc->context)->getPointerTo());
    llvm::Type *llvm_returntype =
        type::TypeName::Int->get_llvm_type(bc->context);
    function_type =
        llvm::FunctionType::get(llvm_returntype, llvm_parameters, false);
  }

  llvm::Function *function = llvm::Function::Create(
      function_type, llvm::Function::ExternalLinkage, name, bc->module);

  llvm::BasicBlock *entry_block = llvm::BasicBlock::Create(bc->context);
  function->getBasicBlockList().push_back(entry_block);
  bc->builder.SetInsertPoint(entry_block);
  if (name == "main") {

    bc->variables.emplace_back(
        std::unordered_map<std::string, llvm::Value *>());

    std::vector<llvm::Type *> llvm_parameters;
    llvm_parameters.emplace_back(llvm::Type::getInt32Ty(bc->context));
    llvm_parameters.emplace_back(
        llvm::Type::getInt8PtrTy(bc->context)->getPointerTo());
    llvm::Type *llvm_returntype =
        type::TypeName::List->get_llvm_type(bc->context);
    llvm::FunctionType *init_function_type =
        llvm::FunctionType::get(llvm_returntype, llvm_parameters, false);

    llvm::FunctionCallee function_callee =
        bc->module.getOrInsertFunction("list_init_main", init_function_type);

    std::string arg_name = parameters[0]->get_name();

    std::vector<llvm::Value *> llvm_args;
    for (auto &arg : function->args()) {
      llvm_args.emplace_back(&arg);
    }

    llvm::AllocaInst *variable_value = bc->builder.CreateAlloca(
        type::TypeName::List->get_llvm_type(bc->context), nullptr,
        llvm::Twine(arg_name));

    bc->builder.CreateStore(bc->builder.CreateCall(function_callee, llvm_args),
                            variable_value);
    bc->variables.back().emplace(arg_name, variable_value);

  } else {
    bc->variables.emplace_back(
        std::unordered_map<std::string, llvm::Value *>());
    unsigned idx = 0;
    for (auto &arg : function->args()) {
      std::string arg_name = parameters[idx++]->get_name();
      llvm::AllocaInst *variable_value = bc->builder.CreateAlloca(
          arg.getType(), nullptr, llvm::Twine(arg_name));
      bc->builder.CreateStore(&arg, variable_value);
      bc->variables.back().emplace(arg_name, variable_value);
    }
  }

  llvm::Value *return_value = blockexpression->build_ir(bc);
  if (*type) {
    bc->builder.CreateRet(return_value);
  } else {
    bc->builder.CreateRetVoid();
  }

  bc->variables.pop_back();
  bc->ast_stack.pop();
  return return_value;
}

auto operator<<(std::ostream &os, const ASTFunction &func) -> std::ostream & {
  os << "func " << func.name << " ";
  if (func.parameters.empty()) {
    os << "Void";
  } else {
    for (auto &para : func.parameters) {
      os << *para << ", ";
    }
  }
  if (func.type) {
    os << "-> " << *func.type << " ";
  }
  os << *func.blockexpression << "\n";
  return os;
};

} // namespace by::ast
