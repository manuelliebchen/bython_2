/*
 * ASTIfExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "ASTIfExpression.hpp"

namespace by::ast {

ASTIfExpression::ASTIfExpression(const std::shared_ptr<peg::Ast>& ast,
								 ASTBlockExpression* parent)
	: ASTExpression(ast, parent)
{
	if (ast->original_name != "IfExpression") {
		throw bad_ast_exeption(
			ast,
			(std::string("IfExpression but was ") + ast->original_name)
				.c_str());
	}

	condition = create_expression(ast->nodes[0], parent);
	block = std::make_shared<ASTBlockExpression>(ast->nodes[1], parent);
	if (ast->nodes.size() > 2) {
		if (ast->nodes[2]->original_name == "IfExpression") {
			alternativ =
				std::make_shared<ASTIfExpression>(ast->nodes[2], parent);
		}
		else if (ast->nodes[2]->original_name == "BlockExpression") {
			alternativ =
				std::make_shared<ASTBlockExpression>(ast->nodes[2], parent);
		}
		else {
			throw bad_ast_exeption(ast, (std::string("IfExpression but was ") +
										 ast->nodes[2]->original_name)
											.c_str());
		}
	}
}

auto ASTIfExpression::build_ir(std::unique_ptr<bc::BuildContext>& bc) const
	-> llvm::Value*
{
	llvm::Value* condition_llvm = condition->build_ir(bc);
	if (alternativ == nullptr) {
		return nullptr;
	}
	llvm::Function* the_function = bc->builder.GetInsertBlock()->getParent();

	llvm::BasicBlock* then_block = llvm::BasicBlock::Create(bc->context);
	llvm::BasicBlock* else_block = llvm::BasicBlock::Create(bc->context);
	llvm::BasicBlock* merge_block = llvm::BasicBlock::Create(bc->context);

	bc->builder.CreateCondBr(condition_llvm, then_block, else_block);
	bc->builder.SetInsertPoint(then_block);

	llvm::Value* block_ir = block->build_ir(bc);

	bc->builder.CreateBr(merge_block);

	then_block = bc->builder.GetInsertBlock();

	the_function->getBasicBlockList().push_back(else_block);
	bc->builder.SetInsertPoint(else_block);

	llvm::Value* alternativ_ir = alternativ->build_ir(bc);

	bc->builder.CreateBr(merge_block);

	else_block = bc->builder.GetInsertBlock();

	the_function->getBasicBlockList().push_back(merge_block);
	bc->builder.SetInsertPoint(merge_block);

	// TODO: get type !
	llvm::PHINode* phi_node =
		bc->builder.CreatePHI(block->get_type().get_llvm_type(bc->context), 2);

	phi_node->addIncoming(block_ir, then_block);
	phi_node->addIncoming(alternativ_ir, else_block);
	return phi_node;
}

void ASTIfExpression::get_dependencies(
	std::unordered_set<std::string>& functions,
	std::unordered_set<std::string>& types) const
{
	condition->get_dependencies(functions, types);
	block->get_dependencies(functions, types);
	if (alternativ != nullptr) {
		alternativ->get_dependencies(functions, types);
	}
}

} /* namespace by::ast */
