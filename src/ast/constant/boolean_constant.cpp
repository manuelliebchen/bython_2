/*
 * ASTBooleanConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include <llvm/IR/Constants.h> // for ConstantInt
#include <string>              // for operator==, string

#include "ast/constant/constant.hpp" // for ASTConstant
#include "bc/build_context.hpp"      // for BuildContext
#include "boolean_constant.hpp"
#include "peglib.h" // for Ast, AstBase

namespace by::ast {
class ASTBlockExpression;
}
namespace by::ast {

ASTBooleanConstant::ASTBooleanConstant(const std::shared_ptr<peg::Ast> &ast,
                                       ASTBlockExpression *parent)
    : ASTConstant(ast, parent, "Bool") {

  value = false;
  if (ast->token == "true") {
    value = true;
  }
}

auto ASTBooleanConstant::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  return value ? llvm::ConstantInt::getTrue(bc->context)
               : llvm::ConstantInt::getFalse(bc->context);
}

} // namespace by::ast
