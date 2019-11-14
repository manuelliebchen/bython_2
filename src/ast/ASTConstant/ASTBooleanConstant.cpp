/*
 * ASTBooleanConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "../../ast/ASTConstant/ASTBooleanConstant.hpp"

namespace by::ast {

ASTBooleanConstant::ASTBooleanConstant(const std::shared_ptr<peg::Ast>& ast)
	: ASTConstant(ast)
{
	if (ast->original_name != "BooleanConstant") {
		throw bad_ast_exeption(
			ast,
			(std::string("BooleanConstant but was ") + ast->original_name)
				.c_str());
	}

	if (ast->token == "true") {
		value = true;
	}
	else {
		value = false;
	}
}

llvm::Value*
ASTBooleanConstant::build_ir(std::unique_ptr<bc::BuildContext>& bc) const
{
	return value ? llvm::ConstantInt::getTrue(bc->context)
				 : llvm::ConstantInt::getFalse(bc->context);
}

void ASTBooleanConstant::get_dependencies(
	std::unordered_set<std::string>& functions,
	std::unordered_set<std::string>& types) const
{
	types.insert("Bool");
}

} /* namespace by::ast */
