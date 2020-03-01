/*
 * ASTIntegerConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "float_constant.hpp"

#include <llvm/ADT/APFloat.h>  // for APFloat
#include <llvm/IR/Constants.h> // for ConstantFP

#include "bc/build_context.hpp" // for BuildContext
#include "constant.hpp"         // for ASTConstant

namespace by::ast {
class ASTBlockExpression;
}

namespace by::ast {

ASTFloatConstant::ASTFloatConstant(const std::shared_ptr<peg::Ast> &ast,
                                   ASTBlockExpression *parent)
    : ASTConstant(ast, parent, "Float") {
  value = std::stof(ast->token);
}

auto ASTFloatConstant::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  return llvm::ConstantFP::get(bc->context, llvm::APFloat(value));
}

} /* namespace by::ast */
