/*
 * ASTVariableExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "variable_expression.hpp"

#include <util/util.hpp>

#include "block_expression.hpp"

namespace by::ast {

ASTVariableExpression::ASTVariableExpression(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  name = util::to_string(ast->nodes[0]);
  if (ast->nodes.size() > 1) {
    next = std::make_shared<ASTVariableExpression>(ast->nodes[1], parent);
  }
}

auto ASTVariableExpression::determine_type(type::variable_map &known_functions)
    -> by::type::TypeName_ptr {
  type = parent->find_variable_type(name);
  return type;
}

auto ASTVariableExpression::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  bc->ast_stack.push(this);
  for (size_t i = bc->variables.size() - 1; i >= 0; --i) {
    if (const auto &vall = bc->variables[i].find(name);
        vall != bc->variables[i].end()) {
      bc->ast_stack.pop();
      return bc->builder.CreateLoad(vall->second);
    }
  }
  throw bc::compiling_error_exeption(ast, "Variable not found!");
}

void ASTVariableExpression::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {}

} /* namespace by::ast */
