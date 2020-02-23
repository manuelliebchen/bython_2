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
    : ASTConstant(ast, parent) {
  if (ast->original_name != "FloatConstant") {
    throw bad_ast_exeption(
        ast,
        (std::string("FloatConstant but was ") + ast->original_name).c_str());
  }
  value = std::stof(ast->token);
  type = by::type::TypeName("Float");
}

auto ASTFloatConstant::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  return llvm::ConstantFP::get(bc->context, llvm::APFloat(value));
}

void ASTFloatConstant::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  types.insert("Float");
}

} /* namespace by::ast */
