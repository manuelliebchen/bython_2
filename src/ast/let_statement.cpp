/*
 * ASTLetStatement.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "let_statement.h"

#include <algorithm>
#include <cctype>
#include <vector>

#include <llvm/ADT/Twine.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <type/type_name.h>

#include "../bc/build_context.h"
#include "ast/ast_error.h"
#include "bc/function_manager.h"
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
  if (*type == *type::TypeName::None) {
    if (!tail.empty()) {
      tailtype = value->determine_type(bc);
      if (tailtype && !tailtype->subtypes.empty()) {
        if (tailtype->name != "List") {
          throw type::type_deduction_exeption(ast, type::TypeName::List,
                                              tailtype);
        }
        type = tailtype->subtypes[0];
        bc->functions.push_back_load(tail, tailtype);
        parent->register_variable(tail, tailtype);
        bc->functions.push_back_load(var, type);
        parent->register_variable(var, type);
      } else {
        throw ast_error(ast, "List has no Subtype.");
      }
    } else {
      type = value->determine_type(bc);
      parent->register_variable(var, type);
    }
  }
  return type;
}

auto ASTLetStatement::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  llvm::Value *rhs_llvm = value->build_ir(bc);
  llvm::AllocaInst *variable_value = bc->builder.CreateAlloca(
      type->get_llvm_type(bc->context), nullptr, llvm::Twine(var));
  if (!tail.empty()) {
    std::string type_name = "record";
    if (type->is_native()) {
      type_name = type->name;
      std::transform(type_name.begin(), type_name.end(), type_name.begin(),
                     ::tolower);
    }

    llvm::Value *head_ir = bc->functions.build(bc, "list_peek_" + type_name,
                                               {tailtype}, {rhs_llvm});

    llvm::Value *tail_ir =
        bc->functions.build(bc, "list_pop", {tailtype}, {rhs_llvm});
    llvm::AllocaInst *tail_value = bc->builder.CreateAlloca(
        tailtype->get_llvm_type(bc->context), nullptr, llvm::Twine(tail));
    bc->builder.CreateStore(tail_ir, tail_value);
    bc->functions.push_back_load(tail, tailtype);

    rhs_llvm = head_ir;
  }

  bc->builder.CreateStore(rhs_llvm, variable_value);
  bc->functions.push_back_load(var, type);

  return variable_value;
}

auto operator<<(std::ostream &os, const ASTLetStatement &let)
    -> std::ostream & {
  os << "let " << let.var;
  if (!let.tail.empty()) {
    os << " : " << let.tail;
  }
  os << " = " << *let.value;
  return os;
}

} /* namespace by::ast */
