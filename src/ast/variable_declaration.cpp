/*
 * VariableDeclaration.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#include <vector>

#include "ast/expression.hpp"
#include "ast/expression/block_expression.hpp"
#include "peglib.h"
#include "type/type_name.hpp"
#include "variable_declaration.hpp"

namespace by {
namespace bc {
struct BuildContext;
} // namespace bc
} // namespace by

namespace by::ast {

ASTVariableDeclaration::ASTVariableDeclaration(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  name = std::to_string(ast->nodes[0]);
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

auto ASTVariableDeclaration::determine_type(type::variable_map &known_functions)
    -> by::type::TypeName_ptr {
  return type;
}

auto ASTVariableDeclaration::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  return nullptr;
}

auto ASTVariableDeclaration::get_name() const -> const std::string & {
  return name;
}

} // namespace by::ast
