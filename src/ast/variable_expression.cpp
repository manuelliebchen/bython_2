/*
 * ASTVariableExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "variable_expression.h"

#include <vector>

#include "../bc/build_context.h"
#include "bc/function_manager.h"
#include "block_expression.h"
#include "expression.h"
#include "type/type_name.h"

namespace by::ast {

ASTVariableExpression::ASTVariableExpression(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  name = std::to_string(ast->nodes[0]);
  if (ast->nodes.size() > 1) {
    next = std::make_shared<ASTVariableExpression>(ast->nodes[1], parent);
  }
}

auto ASTVariableExpression::determine_type(
    std::unique_ptr<bc::BuildContext> &bc) -> by::type::TypeName_ptr {
  if (*type == *type::TypeName::None) {
    if (name != "Null") {
      type = parent->find_variable_type(name);
      if (*type == *type::TypeName::None) {
        type = bc->functions.get_type(name, {});
      }
    } else {
      type = type::TypeName::Null;
    }
  }
  return type;
}

auto ASTVariableExpression::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  return bc->functions.build(bc, name, {}, {});
}

} /* namespace by::ast */
