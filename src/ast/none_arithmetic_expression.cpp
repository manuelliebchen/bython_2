/*
 * ASTNoneArithmeticExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "none_arithmetic_expression.h"

#include <algorithm>
#include <ext/alloc_traits.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <stack>
#include <stddef.h>
#include <vector>

#include "../bc/build_context.h"
#include "ast_error.h"
#include "block_expression.h"
#include "boolean_constant.h"
#include "call_expression.h"
#include "expression.h"
#include "float_constant.h"
#include "if_expression.h"
#include "integer_constant.h"
#include "let_statement.h"
#include "string_constant.h"
#include "variable_expression.h"

namespace by::ast {

ASTNoneArithmeticExpression::ASTNoneArithmeticExpression(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {

  size_t expression_node = 0;
  if (ast->nodes[0]->original_name == "UnaryOperator") {
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
    rhs = std::make_shared<ASTFloatConstant>(expression, parent);
  } else if (expression->original_name == "IntegerConstant") {
    rhs = std::make_shared<ASTIntegerConstant>(expression, parent);
  } else if (expression->original_name == "BooleanConstant") {
    rhs = std::make_shared<ASTBooleanConstant>(expression, parent);
  } else if (expression->original_name == "StringConstant") {
    rhs = std::make_shared<ASTStringConstant>(expression, parent);
  } else {
    throw ast_error(ast, std::string("Expected Expression but was ") +
                             expression->original_name);
  }
}

auto ASTNoneArithmeticExpression::determine_type(type::variable_map &symbols)
    -> by::type::TypeName_ptr {
  type = rhs->determine_type(symbols);
  return type;
}

auto ASTNoneArithmeticExpression::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  bc->ast_stack.push(this);
  llvm::Value *rhs_llvm = rhs->build_ir(bc);
  bc->ast_stack.pop();
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
  throw bc::compiling_error_exeption(ast, "Type not Implementet!");
}

void ASTNoneArithmeticExpression::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  rhs->get_dependencies(functions, types);
}

} /* namespace by::ast */
