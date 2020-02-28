/*
 * ASTIfExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "if_expression.hpp"

namespace by::ast {

ASTIfExpression::ASTIfExpression(const std::shared_ptr<peg::Ast> &ast,
                                 ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  if (ast->original_name != "IfExpression") {
    throw bad_ast_exeption(
        ast,
        (std::string("IfExpression but was ") + ast->original_name).c_str());
  }

  condition = create_expression(ast->nodes[0], parent);
  block = std::make_shared<ASTBlockExpression>(ast->nodes[1], parent);
  if (ast->nodes.size() > 2) {
    if (ast->nodes[2]->original_name == "IfExpression") {
      alternativ = std::make_shared<ASTIfExpression>(ast->nodes[2], parent);
    } else if (ast->nodes[2]->original_name == "BlockExpression") {
      alternativ = std::make_shared<ASTBlockExpression>(ast->nodes[2], parent);
    } else {
      throw bad_ast_exeption(ast, (std::string("IfExpression but was ") +
                                   ast->nodes[2]->original_name)
                                      .c_str());
    }
  }
}

auto ASTIfExpression::determine_type(type::variable_map &symbols)
    -> by::type::TypeName_ptr {
  type::TypeName_ptr condition_type = condition->determine_type(symbols);
  if (*condition_type != type::TypeName("Bool")) {
    throw type::type_deduction_exeption(
        ast, std::make_shared<const type::TypeName>("Bool"), condition_type);
  }
  type = block->determine_type(symbols);
  if (alternativ) {
    type = std::make_shared<const by::type::TypeName>(
        type->deduct_type(*alternativ->determine_type(symbols)));
  }
  return type;
}

auto ASTIfExpression::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  bc->ast_stack.push(this);

  // Generate Condition
  llvm::Value *condition_llvm = condition->build_ir(bc);

  // Getting basick Blocks
  llvm::Function *the_function = bc->builder.GetInsertBlock()->getParent();

  // Getting pre block
  llvm::BasicBlock *entry_block = bc->builder.GetInsertBlock();

  // Generationg Mergeblock
  llvm::BasicBlock *merge_block = llvm::BasicBlock::Create(bc->context);

  // Generating if block
  llvm::BasicBlock *then_block = llvm::BasicBlock::Create(bc->context);
  the_function->getBasicBlockList().push_back(then_block);
  bc->builder.SetInsertPoint(then_block);
  llvm::Value *block_ir = block->build_ir(bc);
  bc->builder.CreateBr(merge_block);
  then_block = bc->builder.GetInsertBlock();

  // Generationg PHI
  bc->builder.SetInsertPoint(merge_block);
  llvm::PHINode *phi_node = nullptr;

  // If return type generate PHI
  if (*type) {
    phi_node = bc->builder.CreatePHI(type->get_llvm_type(bc->context), 2);
    if (block_ir != nullptr) {
      phi_node->addIncoming(block_ir, then_block);
    }
  }

  // If there is an alternativ
  if (alternativ != nullptr) {
    // Generate else block
    llvm::BasicBlock *else_block = llvm::BasicBlock::Create(bc->context);
    the_function->getBasicBlockList().push_back(else_block);
    bc->builder.SetInsertPoint(else_block);
    llvm::Value *alternativ_ir = alternativ->build_ir(bc);
    bc->builder.CreateBr(merge_block);
    else_block = bc->builder.GetInsertBlock();

    bc->builder.SetInsertPoint(entry_block);
    bc->builder.CreateCondBr(condition_llvm, then_block, else_block);

    if (phi_node != nullptr && alternativ_ir != nullptr) {
      phi_node->addIncoming(alternativ_ir, else_block);
    }
  } else {
    bc->builder.SetInsertPoint(entry_block);
    bc->builder.CreateCondBr(condition_llvm, then_block, merge_block);
  }

  // Push merge_block
  the_function->getBasicBlockList().push_back(merge_block);
  bc->builder.SetInsertPoint(merge_block);

  bc->ast_stack.pop();
  return phi_node;
} // namespace by::ast

void ASTIfExpression::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  condition->get_dependencies(functions, types);
  block->get_dependencies(functions, types);
  if (alternativ != nullptr) {
    alternativ->get_dependencies(functions, types);
  }
}

} /* namespace by::ast */
