/*
 * ASTIntegerConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "ASTFloatConstant.hpp"

namespace by::ast {

ASTFloatConstant::ASTFloatConstant(const std::shared_ptr<peg::Ast>& ast)
	: ASTConstant(ast)
{
	if (ast->original_name != "FloatConstant") {
		throw bad_ast_exeption(
			ast,
			(std::string("FloatConstant but was ") + ast->original_name)
				.c_str());
	}
	value = std::stof(ast->token);
}

llvm::Value*
ASTFloatConstant::build_ir(std::unique_ptr<bc::BuildContext>& bc) const
{
	return llvm::ConstantFP::get(bc->context, llvm::APFloat(value));
}

void ASTFloatConstant::get_dependencies(
	std::unordered_set<std::string>& functions,
	std::unordered_set<std::string>& types) const
{
	types.insert("Float");
}

} /* namespace by::ast */
