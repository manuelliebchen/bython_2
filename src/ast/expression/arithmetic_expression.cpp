/*
 * ASTArithmeticExpression.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: jemand
 */

#include "../expression/arithmetic_expression.hpp"

#include "none_arithmetic_expression.hpp"

namespace by::ast {

ASTArithmeticExpression::ASTArithmeticExpression(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent,
    std::shared_ptr<ASTExpression> lhs, std::string BinaryOperator,
    std::shared_ptr<ASTExpression> rhs)
    : ASTExpression(ast, parent), lhs(std::move(lhs)),
      BinaryOperator(std::move(BinaryOperator)), rhs(std::move(rhs)) {
  type = this->lhs->get_type().deduct_type(this->rhs->get_type());
}

auto ASTArithmeticExpression::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  llvm::Value *lhs_llvm = lhs->build_ir(bc);
  llvm::Type *lhs_type = lhs_llvm->getType();
  llvm::Value *rhs_llvm = rhs->build_ir(bc);
  llvm::Type *rhs_type = rhs_llvm->getType();

  if (lhs_type->getTypeID() != rhs_type->getTypeID()) {
    if (lhs_type->isFloatTy() && rhs_type->isIntegerTy()) {
      rhs_llvm = bc->builder.CreateSIToFP(rhs_llvm, lhs_type);
      rhs_type = rhs_llvm->getType();
    } else if (rhs_type->isFloatTy() && lhs_type->isIntegerTy()) {
      lhs_llvm = bc->builder.CreateSIToFP(lhs_llvm, rhs_type);
      lhs_type = lhs_llvm->getType();
    } else {
      throw bc::compiling_error_exeption(ast, "Types missmatch!");
    }
  }

  if (lhs_type->getTypeID() != rhs_type->getTypeID()) {
    throw bc::compiling_error_exeption(ast, "Incompatible Types!");
  }

  llvm::Type *type = lhs_type;

  if (BinaryOperator == "+") {
    if (type->isFloatTy()) {
      return bc->builder.CreateFAdd(lhs_llvm, rhs_llvm);
    }
    if (type->isIntegerTy()) {
      return bc->builder.CreateAdd(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "-") {
    if (type->isFloatTy()) {
      return bc->builder.CreateFSub(lhs_llvm, rhs_llvm);
    }
    if (type->isIntegerTy()) {
      return bc->builder.CreateSub(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "*") {
    if (type->isFloatTy()) {
      return bc->builder.CreateFMul(lhs_llvm, rhs_llvm);
    }
    if (type->isIntegerTy()) {
      return bc->builder.CreateMul(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "/") {
    if (type->isFloatTy()) {
      return bc->builder.CreateFDiv(lhs_llvm, rhs_llvm);
    }
    if (type->isIntegerTy()) {
      return bc->builder.CreateSDiv(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "%") {
    if (type->isIntegerTy()) {
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
    if (type->isFloatTy()) {
      return bc->builder.CreateFCmpOEQ(lhs_llvm, rhs_llvm);
    }
    if (type->isIntegerTy()) {
      return bc->builder.CreateICmpEQ(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "!=") {
    if (type->isFloatTy()) {
      return bc->builder.CreateFCmpONE(lhs_llvm, rhs_llvm);
    }
    if (type->isIntegerTy()) {
      return bc->builder.CreateICmpNE(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == ">") {
    if (type->isFloatTy()) {
      return bc->builder.CreateFCmpOGT(lhs_llvm, rhs_llvm);
    }
    if (type->isIntegerTy()) {
      return bc->builder.CreateICmpSGT(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == ">=") {
    if (type->isFloatTy()) {
      return bc->builder.CreateFCmpOGE(lhs_llvm, rhs_llvm);
    }
    if (type->isIntegerTy()) {
      return bc->builder.CreateICmpSGE(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "<") {
    if (type->isFloatTy()) {
      return bc->builder.CreateFCmpOLT(lhs_llvm, rhs_llvm);
    }
    if (type->isIntegerTy()) {
      return bc->builder.CreateICmpSLT(lhs_llvm, rhs_llvm);
    }
  }
  if (BinaryOperator == "<=") {
    if (type->isFloatTy()) {
      return bc->builder.CreateFCmpOLE(lhs_llvm, rhs_llvm);
    }
    if (type->isIntegerTy()) {
      return bc->builder.CreateICmpSLE(lhs_llvm, rhs_llvm);
    }
  }
  throw bc::compiling_error_exeption(ast, "Type not Implementet!");
}

void ASTArithmeticExpression::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  rhs->get_dependencies(functions, types);
  lhs->get_dependencies(functions, types);
}
} /* namespace by::ast */
// namespace std
