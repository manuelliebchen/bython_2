/*
 * ASTBlockExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "block_expression.hpp"

#include <algorithm>
#include <ext/alloc_traits.h>
#include <stack>
#include <stddef.h>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "bc/build_context.hpp"
#include "expression.hpp"
#include "variable_declaration.hpp"

namespace by::ast {

ASTBlockExpression::ASTBlockExpression(const std::shared_ptr<peg::Ast> &ast,
                                       ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  for (const std::shared_ptr<peg::Ast> &n : ast->nodes) {
    expressions.push_back(create_expression(n, this));
  }
}

ASTBlockExpression::ASTBlockExpression(
    const std::vector<std::shared_ptr<by::ast::ASTVariableDeclaration>>
        &preregistered_variables,
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  for (const auto &variable : preregistered_variables) {
    register_variable(variable->get_name(), variable->get_type());
  }
  for (const std::shared_ptr<peg::Ast> &n : ast->nodes) {
    expressions.push_back(create_expression(n, this));
  }
}

auto ASTBlockExpression::determine_type(type::variable_map &symbols)
    -> by::type::TypeName_ptr {
  if (!expressions.empty()) {
    for (auto &exp : expressions) {
      exp->determine_type(symbols);
    }
    type = expressions.back()->get_type();
    return type;
  }
  return type::TypeName::Void;
}

auto ASTBlockExpression::find_variable_type(const std::string &name) const
    -> by::type::TypeName_ptr {
  auto iterator = known_variables.find(name);
  if (iterator != known_variables.end()) {
    by::type::TypeName_ptr variable_type = iterator->second;
    if (*variable_type) {
      return variable_type;
    }
  } else if (parent != nullptr) {
    return parent->find_variable_type(name);
  }
  throw std::runtime_error("Could not determen type of variable: " + name);
}

auto ASTBlockExpression::register_variable(const std::string &name,
                                           const by::type::TypeName_ptr &type)
    -> bool {
  auto iterator = known_variables.find(name);
  if (iterator == known_variables.end()) {
    known_variables.emplace(name, type);
    return true;
  }
  return false;
}

void ASTBlockExpression::get_dependencies(identifier_set &functions,
                                          identifier_set &types) const {
  for (const auto &exp : expressions) {
    exp->get_dependencies(functions, types);
  }
}

auto ASTBlockExpression::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  bc->ast_stack.push(this);
  bc->variables.emplace_back(std::unordered_map<std::string, llvm::Value *>());

  llvm::Value *ret = nullptr;
  if (!expressions.empty()) {
    for (size_t i = 0; i < expressions.size() - 1; ++i) {
      expressions[i]->build_ir(bc);
    }
    ret = expressions.back()->build_ir(bc);
  }

  bc->variables.pop_back();
  bc->ast_stack.pop();
  return ret;
}

} /* namespace by::ast */
