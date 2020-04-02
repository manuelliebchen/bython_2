/*
 * ASTArithmeticExpression.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: jemand
 */

#include "arithmetic_expression.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <utility>

#include "../bc/build_context.h"
#include "../type/function_type.h"
#include "bc/function_manager.h"
#include "expression.h"
#include "peglib.h"
#include "type/type_name.h"

namespace by::ast {
class ASTBlockExpression;

ASTArithmeticExpression::ASTArithmeticExpression(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent,
    std::shared_ptr<ASTExpression> lhs, std::string BinaryOperator,
    std::shared_ptr<ASTExpression> rhs)
    : ASTExpression(ast, parent), lhs(std::move(lhs)),
      BinaryOperator(std::move(BinaryOperator)), rhs(std::move(rhs)) {}

auto ASTArithmeticExpression::determine_type(
    std::unique_ptr<bc::BuildContext> &bc) -> by::type::TypeName_ptr {
  if (*type == *type::TypeName::None) {
    function_type = std::make_shared<const type::FunctionType>(
        bc->functions.get_type(
            BinaryOperator, {lhs->determine_type(bc), rhs->determine_type(bc)}),
        lhs->get_type(), rhs->get_type());
    type = function_type->return_type;
  }
  return type;
}

auto ASTArithmeticExpression::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  llvm::Value *lhs_llvm = lhs->build_ir(bc);
  llvm::Value *rhs_llvm = rhs->build_ir(bc);

  if (*lhs->get_type() != *lhs->get_type()) {
    lhs_llvm = bc->builder.CreateSIToFP(
        lhs_llvm, lhs->get_type()->get_llvm_type(bc->context));
  }
  if (*rhs->get_type() != *function_type->rhs()) {
    rhs_llvm = bc->builder.CreateSIToFP(
        rhs_llvm, function_type->rhs()->get_llvm_type(bc->context));
  }

  return bc->functions.build(bc, BinaryOperator, function_type->parameters,
                             {lhs_llvm, rhs_llvm});
}

auto operator<<(std::ostream &os, const ASTArithmeticExpression &arit)
    -> std::ostream & {
  os << *arit.lhs << " " << arit.BinaryOperator << " " << *arit.rhs;
  return os;
}

} /* namespace by::ast */
// namespace std
