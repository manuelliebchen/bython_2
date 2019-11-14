/*
 * ASTIfExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "ASTIfExpression.hpp"

namespace by::ast {

ASTIfExpression::ASTIfExpression(const std::shared_ptr<peg::Ast>& ast)
	: ASTExpression(ast)
{
	if (ast->original_name != "IfExpression") {
		throw bad_ast_exeption(
			ast,
			(std::string("IfExpression but was ") + ast->original_name)
				.c_str());
	}

	ruling = create_expression(ast->nodes[0]);
	block = std::make_shared<ASTBlockExpression>(ast->nodes[1]);
	if (ast->nodes.size() > 2) {
		if (ast->nodes[2]->original_name == "IfExpression") {
			alternativ = std::make_shared<ASTIfExpression>(ast->nodes[2]);
		}
		else if (ast->nodes[2]->original_name == "BlockExpression") {
			alternativ = std::make_shared<ASTBlockExpression>(ast->nodes[2]);
		}
		else {
			throw bad_ast_exeption(ast, (std::string("IfExpression but was ") +
										 ast->nodes[2]->original_name)
											.c_str());
		}
	}
}

llvm::Value*
ASTIfExpression::build_ir(std::unique_ptr<bc::BuildContext>& bc) const
{
	llvm::Value* ruling_llvm = ruling->build_ir(bc);
	if (alternativ != nullptr) {
		llvm::BasicBlock* befor_block = llvm::BasicBlock::Create(bc->context);

		llvm::BasicBlock* merge_block = llvm::BasicBlock::Create(bc->context);
		llvm::BasicBlock* then_block = llvm::BasicBlock::Create(bc->context);
		block->build_ir(bc);
		llvm::BasicBlock* else_block = llvm::BasicBlock::Create(bc->context);
		alternativ->build_ir(bc);
		bc->builder.CreateBr(merge_block);

		bc->builder.CreateCondBr(ruling_llvm, then_block, else_block);
	}
}

void ASTIfExpression::get_dependencies(
	std::unordered_set<std::string>& functions,
	std::unordered_set<std::string>& types) const
{
	ruling->get_dependencies(functions, types);
	block->get_dependencies(functions, types);
	if (alternativ != nullptr) {
		alternativ->get_dependencies(functions, types);
	}
}

} /* namespace by::ast */
