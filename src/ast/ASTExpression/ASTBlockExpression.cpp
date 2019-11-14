/*
 * ASTBlockExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "ASTBlockExpression.hpp"

namespace by::ast {

ASTBlockExpression::ASTBlockExpression(const std::shared_ptr<peg::Ast>& ast)
	: ASTExpression(ast)
{
	if (ast->original_name != "BlockExpression") {
		throw bad_ast_exeption(
			ast,
			(std::string("BlockExpression but was ") + ast->original_name)
				.c_str());
	}
	for (const std::shared_ptr<peg::Ast>& n : ast->nodes) {
		expressions.push_back(create_expression(n));
	}
}

void ASTBlockExpression::get_dependencies(identifier_set& functions,
										  identifier_set& types) const
{
	for (const auto& exp : expressions) {
		exp->get_dependencies(functions, types);
	}
}

llvm::Value*
ASTBlockExpression::build_ir(std::unique_ptr<bc::BuildContext>& bc) const
{
	bc->variables.emplace_back(std::unordered_map<std::string, llvm::Value*>());

	llvm::Value* ret = nullptr;
	if (!expressions.empty()) {
		for (size_t i = 0; i < expressions.size() - 1; ++i) {
			expressions[i]->build_ir(bc);
		}
		ret = expressions.back()->build_ir(bc);
	}

	bc->variables.pop_back();
	return ret;
}

} /* namespace by::ast */
