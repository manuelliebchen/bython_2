/*
 * ASTBlockExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "block_expression.hpp"

#include "../variable_declaration.hpp"

namespace by::ast {

ASTBlockExpression::ASTBlockExpression(const std::shared_ptr<peg::Ast> &ast,
                                       ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  if (ast->original_name != "BlockExpression") {
    throw bad_ast_exeption(
        ast,
        (std::string("BlockExpression but was ") + ast->original_name).c_str());
  }
  for (const std::shared_ptr<peg::Ast> &n : ast->nodes) {
    expressions.push_back(create_expression(n, this));
  }
}

ASTBlockExpression::ASTBlockExpression(
    const std::vector<std::shared_ptr<by::ast::ASTVariableDeclaration>>
        &preregistered_variables,
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  if (ast->original_name != "BlockExpression") {
    throw bad_ast_exeption(
        ast,
        (std::string("BlockExpression but was ") + ast->original_name).c_str());
  }
  for (const auto &variable : preregistered_variables) {
    register_variable(variable->get_name(), variable->get_type());
  }
  for (const std::shared_ptr<peg::Ast> &n : ast->nodes) {
    expressions.push_back(create_expression(n, this));
  }
}

auto ASTBlockExpression::find_variable_type(const std::string &name) const
    -> by::type::TypeName {
  auto iterator = known_variables.find(name);
  if (iterator != known_variables.end()) {
    return iterator->second;
  }
  if (parent != nullptr) {
    return parent->find_variable_type(name);
  }
  return by::type::TypeName();
}

auto ASTBlockExpression::register_variable(const std::string &name,
                                           const by::type::TypeName &type)
    -> bool {
  if (!find_variable_type(name)) {
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
  bc->variables.emplace_back(std::unordered_map<std::string, llvm::Value *>());

  llvm::Value *ret = nullptr;
  if (!expressions.empty()) {
    for (size_t i = 0; i < expressions.size() - 1; ++i) {
      expressions[i]->build_ir(bc);
    }
    ret = expressions.back()->build_ir(bc);
  }

  bc->variables.pop_back();
  return ret;
}

} /* namespace by::ast */
