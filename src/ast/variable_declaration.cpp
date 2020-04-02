/*
 * VariableDeclaration.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#include "variable_declaration.h"

#include <vector>

#include "../type/type_name.h"
#include "block_expression.h"
#include "expression.h"
#include "peglib.h"

namespace by {
namespace bc {
class BuildContext;
} // namespace bc

namespace ast {

ASTVariableDeclaration::ASTVariableDeclaration(
    const std::shared_ptr<peg::Ast> &ast)
    : ast(ast) {
  name = std::to_string(ast->nodes[0]);
  type = std::make_shared<const type::TypeName>(ast->nodes[1]);
}

auto ASTVariableDeclaration::get_name() const -> const std::string & {
  return name;
}

auto ASTVariableDeclaration::get_type() const -> by::type::TypeName_ptr {
  return type;
}

auto operator<<(std::ostream &os, const ASTVariableDeclaration &var)
    -> std::ostream & {
  os << var.name << ": " << *var.type;
  return os;
}

} // namespace ast
} // namespace by
