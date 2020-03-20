/*
 * ASTArithmeticExpression.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: jemand
 */

#include "arithmetic_expression.h"

#include <algorithm>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <stack>
#include <utility>

#include "../bc/build_context.h"
#include "../type/function_type.h"
#include "expression.h"
#include "peglib.h"

namespace by {
namespace ast {
class ASTBlockExpression;
} // namespace ast
} // namespace by

namespace by::ast {

ASTArithmeticExpression::ASTArithmeticExpression(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent,
    std::shared_ptr<ASTExpression> lhs, std::string BinaryOperator,
    std::shared_ptr<ASTExpression> rhs)
    : ASTExpression(ast, parent), lhs(std::move(lhs)),
      BinaryOperator(std::move(BinaryOperator)), rhs(std::move(rhs)) {}

auto ASTArithmeticExpression::determine_type(
    std::unique_ptr<bc::BuildContext> &bc) -> by::type::TypeName_ptr {

  auto &funk_builder = bc->find(
      BinaryOperator, {lhs->determine_type(bc), rhs->determine_type(bc)});
  function_type = funk_builder.get_type();
  type = function_type->return_type;
  return type;
}

auto ASTArithmeticExpression::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  llvm::Value *lhs_llvm = lhs->build_ir(bc);
  llvm::Value *rhs_llvm = rhs->build_ir(bc);

  if (*lhs->get_type() != *function_type->lhs()) {
    lhs_llvm = bc->builder.CreateSIToFP(
        lhs_llvm, function_type->lhs()->get_llvm_type(bc->context));
  }
  if (*rhs->get_type() != *function_type->rhs()) {
    rhs_llvm = bc->builder.CreateSIToFP(
        rhs_llvm, function_type->rhs()->get_llvm_type(bc->context));
  }

  return bc->find(BinaryOperator, function_type->parameters)
      .build_ir(bc, {lhs_llvm, rhs_llvm});
}

void ASTArithmeticExpression::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  rhs->get_dependencies(functions, types);
  lhs->get_dependencies(functions, types);
}
} /* namespace by::ast */
// namespace std
