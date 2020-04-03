/*
 * ASTCallExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "call_expression.h"

#include <cstddef>
#include <stdexcept>

#include "../bc/build_context.h"
#include "ast/ast_error.h"
#include "bc/function_manager.h"
#include "expression.h"
#include "type/type_name.h"

namespace by {
namespace ast {
class ASTBlockExpression;
} // namespace ast

namespace ast {

ASTCallExpression::ASTCallExpression(const std::shared_ptr<peg::Ast> &ast,
                                     ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  name = std::to_string(ast->nodes[0]);
  for (size_t i = 1; i < ast->nodes.size(); ++i) {
    arguments.push_back(create_expression(ast->nodes[i], parent));
  }
}

auto ASTCallExpression::determine_type(std::unique_ptr<bc::BuildContext> &bc)
    -> by::type::TypeName_ptr {
  if (*type == *type::TypeName::None) {
    for (auto &arg : arguments) {
      parameter_type.push_back(arg->determine_type(bc));
    }
    type = bc->functions.get_type(name, parameter_type);
  }
  return type;
}

auto ASTCallExpression::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  std::vector<llvm::Value *> llvm_args;
  for (const auto &arg : arguments) {
    llvm_args.emplace_back(arg->build_ir(bc));
  }

  return bc->functions.build(bc, name, parameter_type, llvm_args);
}

auto operator<<(std::ostream &os, const ASTCallExpression &call)
    -> std::ostream & {
  os << call.name << "(";
  if (!call.arguments.empty()) {
    os << *call.arguments.front();
    for (size_t i = 1; i < call.arguments.size(); ++i) {
      os << ", " << *call.arguments[i];
    }
  }
  os << ")";
  return os;
}

} // namespace ast
} // namespace by
