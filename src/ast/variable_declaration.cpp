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
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  name = std::to_string(ast->nodes[0]);
  type = std::make_shared<const type::TypeName>(ast->nodes[1]);

  if (parent != nullptr) {
    parent->register_variable(name, type);
  }
}

auto ASTVariableDeclaration::determine_type(std::unique_ptr<bc::BuildContext> &
                                            /*bc*/) -> by::type::TypeName_ptr {
  return type;
}

auto ASTVariableDeclaration::build_ir(
    std::unique_ptr<bc::BuildContext> & /* bc */) const -> llvm::Value * {
  return nullptr;
}

auto ASTVariableDeclaration::get_name() const -> const std::string & {
  return name;
}

} // namespace ast
} // namespace by
