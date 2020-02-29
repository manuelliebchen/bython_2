/*
 * ASTFunction.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "function.hpp"

#include <llvm/IR/Function.h>

namespace by::ast {

ASTFunction::ASTFunction(const std::shared_ptr<peg::Ast> &ast,
                         ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  if (ast->original_name != "Function") {
    throw bad_ast_exeption(
        ast, (std::string("Function but was ") + ast->original_name).c_str());
  }

  size_t varscount = ast->nodes.size() - 2;
  name = util::to_string(ast->nodes[0]);
  type::TypeName_ptr returntype;
  if (const std::shared_ptr<peg::Ast> &astreturntype = *(ast->nodes.end() - 2);
      astreturntype->original_name == "TypeName") {
    returntype = std::make_shared<type::TypeName>(astreturntype);
    --varscount;
  } else {
    returntype = std::make_shared<type::TypeName>();
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
  std::cerr << "Building " << name << std::endl;

  bc->ast_stack.push(this);

  llvm::Function *function = llvm::Function::Create(
      std::static_pointer_cast<const type::FunctionType>(type)
          ->get_llvm_function_type(bc->context),
      llvm::Function::ExternalLinkage, name, bc->module);

  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(bc->context, "entry", function);
  bc->builder.SetInsertPoint(BB);

  bc->variables.emplace_back(std::unordered_map<std::string, llvm::Value *>());
  unsigned idx = 0;
  for (auto &arg : function->args()) {
    std::string name = parameters[idx++]->get_name();
    arg.setName(name);

    llvm::Type *arg_type = arg.getType();
    llvm::AllocaInst *variable_value = bc->builder.CreateAlloca(arg_type);
    bc->builder.CreateStore(&arg, variable_value);
    bc->variables.back().emplace(name, variable_value);
  }

  //  if (llvm::Value *return_value = blockexpression->build_ir(bc);
  //      return_value != nullptr) {
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
