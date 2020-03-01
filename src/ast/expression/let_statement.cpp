/*
 * ASTLetStatement.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include <algorithm>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <stack>
#include <vector>

#include "ast/expression/block_expression.hpp"
#include "ast/expression/expression.hpp"
#include "bc/build_context.hpp"
#include "let_statement.hpp"

namespace llvm {
class Type;
} // namespace llvm

namespace by::ast {

ASTLetStatement::ASTLetStatement(const std::shared_ptr<peg::Ast> &ast,
                                 ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  var = std::to_string(ast->nodes[0]);
  value = create_expression(ast->nodes[1], parent);
}

auto ASTLetStatement::determine_type(type::variable_map &symbols)
    -> by::type::TypeName_ptr {
  type = value->determine_type(symbols);
  symbols.emplace(var, type);
  parent->register_variable(var, type);
  return type;
}

auto ASTLetStatement::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  bc->ast_stack.push(this);
  llvm::Value *rhs_llvm = value->build_ir(bc);
  llvm::Type *rhs_type = rhs_llvm->getType();

  llvm::AllocaInst *variable_value = bc->builder.CreateAlloca(rhs_type);
  bc->builder.CreateStore(rhs_llvm, variable_value);
  bc->variables.back().emplace(var, variable_value);

  bc->ast_stack.pop();
  return variable_value;
}

void ASTLetStatement::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  value->get_dependencies(functions, types);
}

} /* namespace by::ast */
