/*
 * ASTStringConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "string_constant.h"

#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/Casting.h>

#include "../bc/build_context.h"
#include "constant.h"
#include "type/type_name.h"

namespace by::ast {
class ASTBlockExpression;

ASTStringConstant::ASTStringConstant(const std::shared_ptr<peg::Ast> &ast,
                                     ASTBlockExpression *parent)
    : ASTConstant(ast, parent, "String") {
  value = ast->token;
  auto p = value.find(R"(\n)");
  if (p != std::string::npos) {
    value.replace(p, 2, "\n");
  }
}

auto ASTStringConstant::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  const std::string gname = ".str" + value;
  const auto gdata = llvm::ConstantDataArray::getString(bc->context, value,
                                                        /*AddNull*/ true);

  // Register the declaration statement in the module
  // Note: Name string declaration after string content?
  // Note: Would help avoid duplicates?
  llvm::Constant *text = bc->module.getOrInsertGlobal(gname, gdata->getType());
  const auto gdata_loc = llvm::cast<llvm::GlobalVariable>(text);

  gdata_loc->setInitializer(gdata);

  // Cast to appropriate type and return
  return llvm::ConstantExpr::getBitCast(
      gdata_loc, type::TypeName::String->get_llvm_type(bc->context));
}

auto ASTStringConstant::get_value() const -> std::string { return value; }
} // namespace by::ast
