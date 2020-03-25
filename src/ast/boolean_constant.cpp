/*
 * ASTBooleanConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "boolean_constant.h"

#include <llvm/IR/Constants.h> // for ConstantInt
#include <string>              // for operator==, string

#include "../bc/build_context.h" // for BuildContext
#include "constant.h"            // for ASTConstant
#include "peglib.h"              // for Ast, AstBase

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

} /* namespace by::ast */
