/*
 * VariableDeclaration.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#include "variable_declaration.hpp"

#include "../utillib/utillib.hpp"
namespace by::ast {

ASTVariableDeclaration::ASTVariableDeclaration(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
    : ASTBase(ast, parent) {
  if (ast->original_name != "VariableDeclaration") {
    throw bad_ast_exeption(
        ast, (std::string("VariableDeclaration but was ") + ast->original_name)
                 .c_str());
  }
  name = util::to_string(ast->nodes[0]);
  type = std::make_shared<const type::TypeName>(ast->nodes[1]);

  if (parent != nullptr) {
    parent->register_variable(name, type);
  }
}

void ASTVariableDeclaration::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  types.insert(std::to_string(*type));
}

auto ASTVariableDeclaration::get_name() const -> const std::string & {
  return name;
}

auto ASTVariableDeclaration::get_type() const -> by::type::TypeName_ptr {
  return type;
}

} // namespace by::ast
