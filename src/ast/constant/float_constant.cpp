/*
 * ASTIntegerConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "float_constant.hpp"

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
