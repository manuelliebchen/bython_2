/*
 * ASTFunction.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "function.h"

#include <algorithm>
#include <cstddef>
#include <ext/alloc_traits.h>
#include <iostream>

#include <llvm/ADT/Twine.h>
#include <llvm/ADT/iterator_range.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

#include "../bc/build_context.h"
#include "../type/function_type.h"
#include "../type/type_name.h"
#include "bc/function_manager.h"
#include "block_expression.h"
#include "expression.h"
#include "peglib.h"
#include "variable_declaration.h"

namespace by::ast {

ASTFunction::ASTFunction(const std::shared_ptr<peg::Ast> &ast) : ast(ast) {
  size_t varscount = ast->nodes.size() - 2;
  name = std::to_string(ast->nodes[0]);
  if (const std::shared_ptr<peg::Ast> &astreturntype = *(ast->nodes.end() - 2);
      astreturntype->original_name == "TypeName") {
    type = std::make_shared<const type::TypeName>(astreturntype);
    --varscount;
  } else {
    type = type::TypeName::None;
  }

  std::vector<type::TypeName_ptr> types;
  for (size_t i = 0; i < varscount; ++i) {
    parameters.push_back(
        std::make_shared<ASTVariableDeclaration>(ast->nodes[1 + i]));
    types.push_back(parameters.back()->get_type());
  }
  this->function_type = std::make_shared<const type::FunctionType>(type, types);

  blockexpression = std::make_shared<ASTBlockExpression>(
      parameters, ast->nodes.back(), nullptr);
}

auto ASTFunction::determine_type(std::unique_ptr<bc::BuildContext> &bc)
    -> type::TypeName_ptr {
  type::TypeName_ptr blocktype = blockexpression->determine_type(bc);
  if (*blocktype != *type) {
    throw type::type_deduction_exeption(ast, type,
                                        blockexpression->determine_type(bc));
  }
  return type;
}

auto ASTFunction::get_name() const -> std::string { return name; }

void ASTFunction::insertFunction(
    std::unique_ptr<by::bc::BuildContext> &bc) const {
  if (name != "main") {
    bc->functions.push_back_call(name, this->function_type);
  } else {
    std::vector<llvm::Type *> llvm_parameters{
        llvm::Type::getInt32Ty(bc->context),
        llvm::Type::getInt8PtrTy(bc->context)->getPointerTo()};
    llvm::Type *llvm_returntype =
        type::TypeName::Int->get_llvm_type(bc->context);
    bc->module.getOrInsertFunction(
        name, llvm::FunctionType::get(llvm_returntype, llvm_parameters, false));
  }
}

auto ASTFunction::build_ir(std::unique_ptr<by::bc::BuildContext> &bc) const
    -> llvm::Value * {

  llvm::Function *function = bc->module.getFunction(name);

  llvm::BasicBlock *entry_block = llvm::BasicBlock::Create(bc->context);
  function->getBasicBlockList().push_back(entry_block);
  bc->builder.SetInsertPoint(entry_block);
  if (name == "main") {
    bc->functions.build(bc, "b_init", {}, {});

    std::string arg_name = parameters[0]->get_name();

    llvm::AllocaInst *variable_value = bc->builder.CreateAlloca(
        type::TypeName::List->get_llvm_type(bc->context), nullptr,
        llvm::Twine(arg_name));

    std::vector<llvm::Value *> llvm_args;
    for (auto &arg : function->args()) {
      llvm_args.emplace_back(&arg);
    }

    llvm::FunctionCallee function_callee = bc->module.getOrInsertFunction(
        "list_init_main",
        llvm::FunctionType::get(
            type::TypeName::List->get_llvm_type(bc->context),
            {type::TypeName::Int->get_llvm_type(bc->context),
             llvm::Type::getInt8PtrTy(bc->context)->getPointerTo()},
            false));

    bc->builder.CreateStore(bc->builder.CreateCall(function_callee, llvm_args),
                            variable_value);
    bc->functions.push_back_load(arg_name, type::TypeName::List);

  } else {
    unsigned idx = 0;
    for (auto &arg : function->args()) {
      auto &par = parameters[idx++];
      std::string arg_name = par->get_name();
      llvm::AllocaInst *variable_value = bc->builder.CreateAlloca(
          arg.getType(), nullptr, llvm::Twine(arg_name));
      bc->builder.CreateStore(&arg, variable_value);
      bc->functions.push_back_load(arg_name, par->get_type());
    }
  }

  llvm::Value *return_value = blockexpression->build_ir(bc);

  if (name == "main") {
    bc->functions.build(bc, "b_deinit", {}, {});
  }
  if (*type) {
    bc->builder.CreateRet(return_value);
  } else {
    bc->builder.CreateRetVoid();
  }
  return return_value;
}

auto operator<<(std::ostream &os, const ASTFunction &func) -> std::ostream & {
  os << "func " << func.name;
  if (!func.parameters.empty()) {
    os << " = " << *func.parameters.front();
    for (size_t i = 1; i < func.parameters.size(); ++i) {
      os << ", " << *func.parameters[i];
    }
  }
  if (func.type) {
    os << " -> " << *func.type << " ";
  }
  os << *func.blockexpression << "\n";
  return os;
};

} // namespace by::ast
