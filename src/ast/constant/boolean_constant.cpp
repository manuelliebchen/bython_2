/*
 * ASTBooleanConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "boolean_constant.hpp"

namespace by::ast {

ASTBooleanConstant::ASTBooleanConstant(const std::shared_ptr<peg::Ast> &ast,
                                       ASTBlockExpression *parent)
    : ASTConstant(ast, parent) {
  if (ast->original_name != "BooleanConstant") {
    throw bad_ast_exeption(
        ast,
        (std::string("BooleanConstant but was ") + ast->original_name).c_str());
  }

  value = false;
  if (ast->token == "true") {
    value = true;
  }
  type = type::TypeName("Bool");
}

auto ASTBooleanConstant::determine_type(
    const type::function_map &known_functions) -> by::type::TypeName {
  type = type::TypeName("Bool");
  return type;
}

auto ASTBooleanConstant::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  return value ? llvm::ConstantInt::getTrue(bc->context)
               : llvm::ConstantInt::getFalse(bc->context);
}

void ASTBooleanConstant::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  types.insert("Bool");
}

} /* namespace by::ast */
