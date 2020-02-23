/*
 * ASTConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "constant.hpp"

#include "boolean_constant.hpp"
#include "float_constant.hpp"
#include "integer_constant.hpp"
#include "string_constant.hpp"

namespace by::ast {

ASTConstant::ASTConstant(const std::shared_ptr<peg::Ast> &ast,
                         ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {}

auto operator<<(std::ostream &os, const ASTConstant &constant)
    -> std::ostream & {
  if (const auto *dyn = dynamic_cast<const ASTIntegerConstant *>(&constant)) {
    os << *dyn;
  } else if (const auto *dyn =
                 dynamic_cast<const ASTFloatConstant *>(&constant)) {
    os << *dyn;
  } else if (const auto *dyn =
                 dynamic_cast<const ASTBooleanConstant *>(&constant)) {
    os << *dyn;
  } else if (const auto *dyn =
                 dynamic_cast<const ASTStringConstant *>(&constant)) {
    os << *dyn;
  } else {
    os << *dynamic_cast<const ASTExpression *>(&constant);
  }
  return os;
}

} /* namespace by::ast */
