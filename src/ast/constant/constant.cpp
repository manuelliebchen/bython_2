/*
 * ASTConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "constant.hpp"
#include "ast/expression/expression.hpp" // for operator<<, ASTExpression
#include "boolean_constant.hpp"          // for operator<<, ASTBooleanConstant
#include "float_constant.hpp"            // for operator<<, ASTFloatConstant
#include "integer_constant.hpp"          // for operator<<, ASTIntegerConstant
#include "string_constant.hpp"           // for operator<<, ASTStringConstant
#include <utility>                       // for move
namespace by::ast {
class ASTBlockExpression;
}

namespace by::ast {

ASTConstant::ASTConstant(const std::shared_ptr<peg::Ast> &ast,
                         ASTBlockExpression *parent, std::string const_string)
    : ASTExpression(ast, parent), const_string(std::move(const_string)) {}

auto ASTConstant::determine_type(type::variable_map &symbols)
    -> by::type::TypeName_ptr {
  type = std::make_shared<const type::TypeName>(type::TypeName(const_string));
  return type;
}

void ASTConstant::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  types.insert(const_string);
}

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
