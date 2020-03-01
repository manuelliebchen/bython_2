/*
 * ASTIntegerConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "integer_constant.h"

#include <limits>                 // for numeric_limits, numeric_limits<...
#include <llvm/IR/Constants.h>    // for ConstantInt
#include <llvm/IR/DerivedTypes.h> // for IntegerType

#include "../bc/build_context.h" // for BuildContext
#include "constant.h"            // for ASTConstant

namespace by::ast {
class ASTBlockExpression;
}

namespace by::ast {

ASTIntegerConstant::ASTIntegerConstant(const std::shared_ptr<peg::Ast> &ast,
                                       ASTBlockExpression *parent)
    : ASTConstant(ast, parent, "Int") {
  value = std::stoi(ast->token);
}

auto ASTIntegerConstant::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  auto llvm_int_type =
      llvm::IntegerType::get(bc->context, std::numeric_limits<int>::digits + 1);

  return llvm::ConstantInt::getSigned(llvm_int_type, value);
}

} /* namespace by::ast */
