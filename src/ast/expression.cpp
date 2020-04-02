/*
 * ASTExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "expression.h"

#include <algorithm>
#include <ext/alloc_traits.h>
#include <string>
#include <utility>
#include <vector>

#include "../type/type_name.h"
#include "arithmetic_expression.h"
#include "block_expression.h"
#include "call_expression.h"
#include "constant.h"
#include "expression_chain.h"
#include "if_expression.h"
#include "let_statement.h"
#include "none_arithmetic_expression.h"
#include "peglib.h"
#include "variable_expression.h"

namespace by::ast {
ASTExpression::ASTExpression(std::shared_ptr<peg::Ast> ast,
                             ASTBlockExpression *parent)
    : ast(std::move(ast)), parent(parent), type(type::TypeName::None) {}

auto ASTExpression::get_type() const -> by::type::TypeName_ptr { return type; }

auto operator<<(std::ostream &os, const by::ast::ASTExpression &exp)
    -> std::ostream & {

  if (const auto *dyn =
          dynamic_cast<const ASTNoneArithmeticExpression *>(&exp)) {
    os << *dyn;
  } else if (const auto *dyn =
                 dynamic_cast<const ASTArithmeticExpression *>(&exp)) {
    os << *dyn;
  } else if (const auto *dyn = dynamic_cast<const ExpressionChain *>(&exp)) {
    os << *dyn;
  } else if (const auto *dyn = dynamic_cast<const ASTCallExpression *>(&exp)) {
    os << *dyn;
  } else if (const auto *dyn = dynamic_cast<const ASTIfExpression *>(&exp)) {
    os << *dyn;
  } else if (const auto *dyn = dynamic_cast<const ASTBlockExpression *>(&exp)) {
    os << *dyn;
  } else if (const auto *dyn = dynamic_cast<const ASTLetStatement *>(&exp)) {
    os << *dyn;
  } else if (const auto *dyn = dynamic_cast<const ASTConstant *>(&exp)) {
    os << *dyn;
  } else if (const auto *dyn =
                 dynamic_cast<const ASTVariableExpression *>(&exp)) {
    os << *dyn;
  }
  return os;
};

auto ASTExpression::create_expression(const std::shared_ptr<peg::Ast> &ast,
                                      ASTBlockExpression *parent)
    -> std::shared_ptr<by::ast::ASTExpression> {
  if (ast->nodes.size() == 1) {
    return std::make_shared<by::ast::ASTNoneArithmeticExpression>(ast->nodes[0],
                                                                  parent);
  }

  std::shared_ptr<peg::Ast> ast_ptr = ast;
  std::vector<std::string> operators;
  std::vector<std::shared_ptr<ASTExpression>> expressions;

  while (ast_ptr->nodes.size() > 1) {
    expressions.emplace_back(
        std::make_shared<by::ast::ASTNoneArithmeticExpression>(
            ast_ptr->nodes[0], parent));
    operators.emplace_back(ast_ptr->nodes[1]->token);

    ast_ptr = ast_ptr->nodes[2];
  }

  expressions.emplace_back(
      std::make_shared<by::ast::ASTNoneArithmeticExpression>(ast_ptr->nodes[0],
                                                             parent));

  return std::make_shared<ExpressionChain>(ast, parent, expressions, operators);
}

} // namespace by::ast
