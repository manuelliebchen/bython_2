/*
 * ASTCallExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "call_expression.h"

#include <algorithm>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <stack>
#include <stddef.h>

#include "../bc/build_context.h"
#include "../type/function_type.h"
#include "expression.h"

namespace by {
namespace ast {
class ASTBlockExpression;
} // namespace ast
} // namespace by

namespace by::ast {

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
  for (auto &arg : arguments) {
    arg->determine_type(bc);
  }
  try {
    type = bc->find(name).get_type()->return_type;
  } catch (std::out_of_range &oor) {
    throw ast_error(ast, "Function not found: " + name);
  }
  return type;
}

auto ASTCallExpression::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  std::vector<llvm::Value *> llvm_args;
  for (const auto &arg : arguments) {
    llvm_args.emplace_back(arg->build_ir(bc));
  }

  return bc->find(name).build_ir(bc, llvm_args);
}

void ASTCallExpression::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  functions.insert(name);
  for (const auto &arg : arguments) {
    arg->get_dependencies(functions, types);
  }
}

} /* namespace by::ast */
