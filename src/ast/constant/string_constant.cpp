/*
 * ASTStringConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "string_constant.hpp"

namespace by::ast {

ASTStringConstant::ASTStringConstant(const std::shared_ptr<peg::Ast> &ast,
                                     ASTBlockExpression *parent)
    : ASTConstant(ast, parent, "String") {
  if (ast->original_name != "StringConstant") {
    throw bad_ast_exeption(
        ast,
        (std::string("StringConstant but was ") + ast->original_name).c_str());
  }

  value = ast->token;
}

auto ASTStringConstant::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  const auto gname = ".str" + value;

  const auto gdata = llvm::ConstantDataArray::getString(bc->context, value,
                                                        /*AddNull*/ true);

  // Register the declaration statement in the module
  // Note: Name string declaration after string content?
  // Note: Would help avoid duplicates?
  llvm::Constant *text = bc->module.getOrInsertGlobal(gname, gdata->getType());
  const auto gdata_loc = llvm::cast<llvm::GlobalVariable>(text);

  gdata_loc->setInitializer(gdata);

  // Cast to appropriate type and return
  return llvm::ConstantExpr::getBitCast(gdata_loc,
                                        llvm::Type::getInt8PtrTy(bc->context));
}
} /* namespace by::ast */
