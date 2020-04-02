/*
 * ASTNoneArithmeticExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "none_arithmetic_expression.h"

#include <cstddef>
#include <ext/alloc_traits.h>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include <bc/build_context.h>
#include <type/type_name.h>

#include "ast_error.h"
#include "block_expression.h"
#include "call_expression.h"
#include "constant.h"
#include "expression.h"
#include "expression_chain.h"
#include "if_expression.h"
#include "let_statement.h"
#include "variable_expression.h"

namespace by::ast {

ASTNoneArithmeticExpression::ASTNoneArithmeticExpression(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {

  size_t expression_node = 0;
  if (ast->nodes[0]->original_name == "Operator") {
    UnaryOperator = ast->nodes[0]->token;
    expression_node++;
  } else {
    UnaryOperator = "";
  }

  const std::shared_ptr<peg::Ast> &expression = ast->nodes[expression_node];
  if (expression->original_name == "Expression") {
    rhs = create_expression(expression, parent);
  } else if (expression->original_name == "BlockExpression") {
    rhs = std::make_shared<ASTBlockExpression>(expression, parent);
  } else if (expression->original_name == "CallExpression") {
    rhs = std::make_shared<ASTCallExpression>(expression, parent);
  } else if (expression->original_name == "LetStatement") {
    rhs = std::make_shared<ASTLetStatement>(expression, parent);
  } else if (expression->original_name == "IfExpression") {
    rhs = std::make_shared<ASTIfExpression>(expression, parent);
  } else if (expression->original_name == "VariableExpression") {
    rhs = std::make_shared<ASTVariableExpression>(expression, parent);
  } else if (expression->original_name == "FloatConstant") {
    rhs = std::make_shared<ASTConstant<float>>(expression, parent);
  } else if (expression->original_name == "IntegerConstant") {
    rhs = std::make_shared<ASTConstant<int>>(expression, parent);
  } else if (expression->original_name == "BooleanConstant") {
    rhs = std::make_shared<ASTConstant<bool>>(expression, parent);
  } else if (expression->original_name == "StringConstant") {
    rhs = std::make_shared<ASTConstant<std::string>>(expression, parent);
  } else {
    throw ast_error(ast, std::string("Expected Expression but was ") +
                             expression->original_name);
  }
}

auto ASTNoneArithmeticExpression::determine_type(
    std::unique_ptr<bc::BuildContext> &bc) -> by::type::TypeName_ptr {
  if (*type == *type::TypeName::None) {
    type = rhs->determine_type(bc);
  }
  return type;
}

auto ASTNoneArithmeticExpression::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  llvm::Value *rhs_llvm = rhs->build_ir(bc);
  if (UnaryOperator == "-") {
    if (rhs_llvm->getType()->isFloatTy()) {
      return bc->builder.CreateFNeg(rhs_llvm);
    }
    if (rhs_llvm->getType()->isIntegerTy()) {
      return bc->builder.CreateNeg(rhs_llvm);
    }
  }
  if (UnaryOperator == "!") {
    return bc->builder.CreateNot(rhs_llvm);
  }
  if (UnaryOperator.empty()) {
    return rhs_llvm;
  }
  throw ast_error(ast, "Unimplemented Unary Operator: " + UnaryOperator);
}

auto operator<<(std::ostream &os, const ASTNoneArithmeticExpression &none)
    -> std::ostream & {
  os << none.UnaryOperator;
  if (const auto *dyn = dynamic_cast<const ExpressionChain *>(none.rhs.get())) {
    os << "(" << *dyn << ")";
  } else {
    os << *none.rhs;
  }
  return os;
}

} /* namespace by::ast */
