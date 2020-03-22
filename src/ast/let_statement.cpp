/*
 * ASTLetStatement.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "let_statement.h"

#include <algorithm>
#include <ctype.h>
#include <llvm/ADT/Twine.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <type/type_name.h>
#include <vector>

#include "../bc/build_context.h"
#include "ast/ast_error.h"
#include "bc/function_build.h"
#include "block_expression.h"
#include "expression.h"

namespace by::ast {

ASTLetStatement::ASTLetStatement(const std::shared_ptr<peg::Ast> &ast,
                                 ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  var = std::to_string(ast->nodes[0]);
  if (ast->nodes.size() > 2) {
    tail = std::to_string(ast->nodes[1]);
    value = create_expression(ast->nodes[2], parent);
  } else {
    value = create_expression(ast->nodes[1], parent);
  }
}

auto ASTLetStatement::determine_type(std::unique_ptr<bc::BuildContext> &bc)
    -> by::type::TypeName_ptr {
  if (tail != "") {
    tailtype = value->determine_type(bc);
    if (tailtype && !tailtype->subtypes.empty()) {
      type = tailtype->subtypes[0];
      if (tailtype->name != "List") {
        throw type::type_deduction_exeption(ast, type::TypeName::List,
                                            tailtype);
      }
      bc->push_back_load(tail, tailtype);
      parent->register_variable(tail, tailtype);
      bc->push_back_load(var, type);
      parent->register_variable(var, type);
    } else {
      throw ast_error(ast, "List has no Subtype.");
    }
  } else {
    type = value->determine_type(bc);
    parent->register_variable(var, type);
  }
  return type;
}

auto ASTLetStatement::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  llvm::Value *rhs_llvm = value->build_ir(bc);
  llvm::AllocaInst *variable_value = bc->builder.CreateAlloca(
      type->get_llvm_type(bc->context), nullptr, llvm::Twine(var));
  if (tail != "") {
    std::string type_name = "record";
    if (type->is_native()) {
      type_name = type->name;
      std::transform(type_name.begin(), type_name.end(), type_name.begin(),
                     ::tolower);
    }

    llvm::Value *head_ir =
        bc->find("list_peek_" + type_name).build_ir(bc, {rhs_llvm});

    llvm::Value *tail_ir = bc->find("list_pop").build_ir(bc, {rhs_llvm});
    llvm::AllocaInst *tail_value = bc->builder.CreateAlloca(
        tailtype->get_llvm_type(bc->context), nullptr, llvm::Twine(tail));
    bc->builder.CreateStore(tail_ir, tail_value);
    bc->push_back_load(tail, tailtype);

    rhs_llvm = head_ir;
  }

  bc->builder.CreateStore(rhs_llvm, variable_value);
  bc->push_back_load(var, type);

  return variable_value;
}

} /* namespace by::ast */
