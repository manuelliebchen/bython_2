/*
 * ASTCallExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "call_expression.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>

#include "utillib.hpp"

namespace by::ast {

ASTCallExpression::ASTCallExpression(const std::shared_ptr<peg::Ast> &ast,
                                     ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  if (ast->original_name != "CallExpression") {
    throw bad_ast_exeption(
        ast,
        (std::string("CallExpression but was ") + ast->original_name).c_str());
  }
  name = util::to_string(ast->nodes[0]);
  for (size_t i = 1; i < ast->nodes.size(); ++i) {
    arguments.push_back(create_expression(ast->nodes[i], parent));
  }
}

auto ASTCallExpression::determine_type(type::variable_map &symbols)
    -> by::type::TypeName_ptr {
  for (auto &arg : arguments) {
    arg->determine_type(symbols);
  }
  type = symbols.at(name);
  return type;
}

auto ASTCallExpression::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  bc->ast_stack.push(this);

  type::FunctionType_ptr functiontype =
      std::static_pointer_cast<const by::type::FunctionType>(
          bc->symbols.at(name));

  llvm::FunctionType *llvm_functype =
      functiontype->get_llvm_function_type(bc->context);
  llvm::FunctionCallee function_callee =
      bc->module.getOrInsertFunction(name, llvm_functype);

  std::vector<llvm::Value *> llvm_args;
  for (const auto &arg : arguments) {
    llvm_args.emplace_back(arg->build_ir(bc));
  }

  bc->ast_stack.pop();
  return bc->builder.CreateCall(function_callee, llvm_args);
}

void ASTCallExpression::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  functions.insert(name);
  for (const auto &arg : arguments) {
    arg->get_dependencies(functions, types);
  }
}

} /* namespace by::ast */
