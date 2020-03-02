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
const std::multimap<std::string, type::BinaryOperator>
    ASTArithmeticExpression::operators = {
        {"==", {{"Bool"}, {"Int"}, {"Int"}}},
        {"==", {{"Bool"}, {"Float"}, {"Float"}}},
        {"==", {{"Bool"}, {"Bool"}, {"Bool"}}},
        {"!=", {{"Bool"}, {"Int"}, {"Int"}}},
        {"!=", {{"Bool"}, {"Float"}, {"Float"}}},
        {"!=", {{"Bool"}, {"Bool"}, {"Bool"}}},

        {">", {{"Bool"}, {"Int"}, {"Int"}}},
        {">", {{"Bool"}, {"Float"}, {"Float"}}},
        {">=", {{"Bool"}, {"Int"}, {"Int"}}},
        {">=", {{"Bool"}, {"Float"}, {"Float"}}},

        {"<", {{"Bool"}, {"Int"}, {"Int"}}},
        {"<", {{"Bool"}, {"Float"}, {"Float"}}},
        {"<=", {{"Bool"}, {"Int"}, {"Int"}}},
        {"<=", {{"Bool"}, {"Float"}, {"Float"}}},

        {"+", {{"Int"}, {"Int"}, {"Int"}}},
        {"+", {{"Float"}, {"Float"}, {"Float"}}},
        {"-", {{"Int"}, {"Int"}, {"Int"}}},
        {"-", {{"Float"}, {"Float"}, {"Float"}}},

        {"*", {{"Int"}, {"Int"}, {"Int"}}},
        {"*", {{"Float"}, {"Float"}, {"Float"}}},
        {"/", {{"Int"}, {"Int"}, {"Int"}}},
        {"/", {{"Float"}, {"Float"}, {"Float"}}},
        {"%", {{"Int"}, {"Int"}, {"Int"}}},
        {"%", {{"Float"}, {"Float"}, {"Float"}}},

        {"&&", {{"Bool"}, {"Bool"}, {"Bool"}}},
        {"||", {{"Bool"}, {"Bool"}, {"Bool"}}},
};

ASTArithmeticExpression::ASTArithmeticExpression(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent,
    std::shared_ptr<ASTExpression> lhs, std::string BinaryOperator,
    std::shared_ptr<ASTExpression> rhs)
    : ASTExpression(ast, parent), lhs(std::move(lhs)),
      BinaryOperator(std::move(BinaryOperator)), rhs(std::move(rhs)) {}

auto ASTArithmeticExpression::determine_type(type::variable_map &symbols)
    -> by::type::TypeName_ptr {

  operation_type = std::make_shared<const type::TypeName>(
      this->lhs->determine_type(symbols)->deduct_type(
          *(this->rhs->determine_type(symbols))));

  auto bin = operators.equal_range(BinaryOperator);
  for (auto it = bin.first; it != bin.second; ++it) {
    type::BinaryOperator binop = it->second;
    if (binop.lhs == *operation_type) {
      type = std::make_shared<const type::FunctionType>(binop);
      return type;
    }
  }
  throw type::type_deduction_exeption(ast, operation_type,
                                      type::TypeName::None);
}

auto ASTArithmeticExpression::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  bc->ast_stack.push(this);

  llvm::Value *lhs_llvm = lhs->build_ir(bc);
  llvm::Type *lhs_type = lhs_llvm->getType();
  llvm::Value *rhs_llvm = rhs->build_ir(bc);
  llvm::Type *rhs_type = rhs_llvm->getType();

  if (lhs_type->getTypeID() != rhs_type->getTypeID()) {
    if (lhs_type->isFloatTy() && rhs_type->isIntegerTy()) {
      rhs_llvm = bc->builder.CreateSIToFP(rhs_llvm, lhs_type);
    } else if (rhs_type->isFloatTy() && lhs_type->isIntegerTy()) {
      lhs_llvm = bc->builder.CreateSIToFP(lhs_llvm, rhs_type);
    } else {
      throw by::type::type_deduction_exeption(ast, lhs->get_type(),
                                              rhs->get_type());
    }
  }

  bc->ast_stack.pop();
  llvm::Type *llvm_type = operation_type->get_llvm_type(bc->context);

  if (BinaryOperator == "+") {
    if (llvm_type->isFloatTy()) {
      return bc->builder.CreateFAdd(lhs_llvm, rhs_llvm);
    }
    if (llvm_type->isIntegerTy()) {
      return bc->builder.CreateAdd(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "-") {
    if (llvm_type->isFloatTy()) {
      return bc->builder.CreateFSub(lhs_llvm, rhs_llvm);
    }
    if (llvm_type->isIntegerTy()) {
      return bc->builder.CreateSub(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "*") {
    if (llvm_type->isFloatTy()) {
      return bc->builder.CreateFMul(lhs_llvm, rhs_llvm);
    }
    if (llvm_type->isIntegerTy()) {
      return bc->builder.CreateMul(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "/") {
    if (llvm_type->isFloatTy()) {
      return bc->builder.CreateFDiv(lhs_llvm, rhs_llvm);
    }
    if (llvm_type->isIntegerTy()) {
      return bc->builder.CreateSDiv(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "%") {
    if (llvm_type->isIntegerTy()) {
      return bc->builder.CreateSRem(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "&&") {
    return bc->builder.CreateAnd(lhs_llvm, rhs_llvm);
  }
  if (BinaryOperator == "||") {
    return bc->builder.CreateOr(lhs_llvm, rhs_llvm);
  }
  if (BinaryOperator == "==") {
    if (llvm_type->isFloatTy()) {
      return bc->builder.CreateFCmpOEQ(lhs_llvm, rhs_llvm);
    }
    if (llvm_type->isIntegerTy()) {
      return bc->builder.CreateICmpEQ(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "!=") {
    if (llvm_type->isFloatTy()) {
      return bc->builder.CreateFCmpONE(lhs_llvm, rhs_llvm);
    }
    if (llvm_type->isIntegerTy()) {
      return bc->builder.CreateICmpNE(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == ">") {
    if (llvm_type->isFloatTy()) {
      return bc->builder.CreateFCmpOGT(lhs_llvm, rhs_llvm);
    }
    if (llvm_type->isIntegerTy()) {
      return bc->builder.CreateICmpSGT(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == ">=") {
    if (llvm_type->isFloatTy()) {
      return bc->builder.CreateFCmpOGE(lhs_llvm, rhs_llvm);
    }
    if (llvm_type->isIntegerTy()) {
      return bc->builder.CreateICmpSGE(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "<") {
    if (llvm_type->isFloatTy()) {
      return bc->builder.CreateFCmpOLT(lhs_llvm, rhs_llvm);
    }
    if (llvm_type->isIntegerTy()) {
      return bc->builder.CreateICmpSLT(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "<=") {
    if (llvm_type->isFloatTy()) {
      return bc->builder.CreateFCmpOLE(lhs_llvm, rhs_llvm);
    }
    if (llvm_type->isIntegerTy()) {
      return bc->builder.CreateICmpSLE(lhs_llvm, rhs_llvm);
    }
  }
  throw ast_error(ast, "Unimplemented Binary Operator.");
}

void ASTArithmeticExpression::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  rhs->get_dependencies(functions, types);
  lhs->get_dependencies(functions, types);
}
} /* namespace by::ast */
// namespace std