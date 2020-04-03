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

  if (ast->nodes[0]->original_name == "Operator") {
    UnaryOperator = ast->nodes[0]->token;
  }

  const std::shared_ptr<peg::Ast> &expression = ast->nodes.back();
  if (expression->original_name == "Expression") {
    rhs = ASTExpression::create_expression(expression, parent);
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
    if (!UnaryOperator.empty()) {
      type = bc->functions.get_type(UnaryOperator, {type});
    }
  }
  return type;
}

auto ASTNoneArithmeticExpression::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  if (UnaryOperator.empty()) {
    return rhs->build_ir(bc);
  };
  return bc->functions.build(bc, UnaryOperator, {rhs->determine_type(bc)},
                             {rhs->build_ir(bc)});
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
