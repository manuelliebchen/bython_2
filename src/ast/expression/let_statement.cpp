/*
 * ASTLetStatement.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "let_statement.hpp"

#include "utillib.hpp"

namespace by::ast {

ASTLetStatement::ASTLetStatement(const std::shared_ptr<peg::Ast> &ast,
                                 ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  if (ast->original_name != "LetStatement") {
    throw bad_ast_exeption(
        ast,
        (std::string("LetStatement but was ") + ast->original_name).c_str());
  }
  var = util::to_string(ast->nodes[0]);
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
  llvm::StoreInst *store = bc->builder.CreateStore(rhs_llvm, variable_value);
  bc->variables.back().emplace(var, bc->builder.CreateLoad(variable_value));

  bc->ast_stack.pop();
  return store;
}

void ASTLetStatement::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  value->get_dependencies(functions, types);
}

} /* namespace by::ast */
