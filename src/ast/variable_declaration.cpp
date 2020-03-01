/*
 * VariableDeclaration.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#include <util/util.hpp>
#include <vector>

#include "ast/base.hpp"
#include "ast/expression/block_expression.hpp"
#include "peglib.h"
#include "type/type_name.hpp"
#include "variable_declaration.hpp"

namespace by::ast {

ASTVariableDeclaration::ASTVariableDeclaration(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
    : ASTBase(ast, parent) {
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
