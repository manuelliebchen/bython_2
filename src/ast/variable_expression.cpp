/*
 * ASTVariableExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "variable_expression.h"

#include <algorithm>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <stack>
#include <stddef.h>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../bc/build_context.h"
#include "block_expression.h"
#include "expression.h"

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
  if (name != "Null") {
    type = parent->find_variable_type(name);
  } else {
    type = type::TypeName::Null;
  }
  return type;
}

auto ASTVariableExpression::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  if (name != "Null") {
    return bc->find(name).build_ir(bc, {});
  } else {
    return llvm::ConstantPointerNull::get(
        (llvm::PointerType *)type::TypeName::Null->get_llvm_type(bc->context));
  }
}

void ASTVariableExpression::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {}

} /* namespace by::ast */
