/*
 * ASTLetStatement.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "let_statement.h"

#include <algorithm>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <stack>
#include <vector>

#include "../bc/build_context.h"
#include "block_expression.h"
#include "call_expression.h"
#include "expression.h"
#include <type/type_name.h>

namespace llvm {
class Type;
} // namespace llvm

namespace by::ast {

ASTLetStatement::ASTLetStatement(const std::shared_ptr<peg::Ast> &ast,
                                 ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  if (ast->nodes.size() > 2) {
    var = std::to_string(ast->nodes[0]);
    tail = std::to_string(ast->nodes[1]);
    value = create_expression(ast->nodes[2], parent);
  } else {
    var = std::to_string(ast->nodes[0]);
    value = create_expression(ast->nodes[1], parent);
  }
}

auto ASTLetStatement::determine_type(type::variable_map &symbols)
    -> by::type::TypeName_ptr {
  if (tail != "") {
    tailtype = value->determine_type(symbols);
    type = std::make_shared<type::TypeName>(tailtype->subtypes[0]);
    if (tailtype->name != "llist") {
      throw type::type_deduction_exeption(ast, type::TypeName::llist, tailtype);
    }
    symbols.emplace(tail, tailtype);
    parent->register_variable(tail, tailtype);
    symbols.emplace(var, type);
    parent->register_variable(var, type);
  } else {
    type = value->determine_type(symbols);
    symbols.emplace(var, type);
    parent->register_variable(var, type);
  }
  return type;
}

auto ASTLetStatement::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  bc->ast_stack.push(this);
  llvm::Value *rhs_llvm = value->build_ir(bc);
  llvm::AllocaInst *variable_value = bc->builder.CreateAlloca(
      type->get_llvm_type(bc->context), nullptr, llvm::Twine(var));
  if (tail != "") {
    llvm::Value *head_ir =
        bc::build_internal_call(bc, "llist_peek", {rhs_llvm});

    bc->builder.CreateStore(bc->builder.CreateLoad(head_ir), variable_value);
    bc->variables.back().emplace(var, variable_value);

    llvm::Value *tail_ir = bc::build_internal_call(bc, "llist_pop", {rhs_llvm});
    llvm::AllocaInst *tail_value = bc->builder.CreateAlloca(
        tailtype->get_llvm_type(bc->context), nullptr, llvm::Twine(tail));
    bc->builder.CreateStore(tail_ir, tail_value);
    bc->variables.back().emplace(tail, tail_value);

  } else {
    bc->builder.CreateStore(rhs_llvm, variable_value);
    bc->variables.back().emplace(var, variable_value);
  }

  bc->ast_stack.pop();
  return variable_value;
}

void ASTLetStatement::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  value->get_dependencies(functions, types);
}

} /* namespace by::ast */
