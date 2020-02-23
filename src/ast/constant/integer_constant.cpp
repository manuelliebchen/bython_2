/*
 * ASTIntegerConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "integer_constant.hpp"

namespace by::ast {

ASTIntegerConstant::ASTIntegerConstant(const std::shared_ptr<peg::Ast>& ast,
									   ASTBlockExpression* parent)
	: ASTConstant(ast, parent)
{
	if (ast->original_name != "IntegerConstant") {
		throw bad_ast_exeption(
			ast,
			(std::string("IntegerConstant but was ") + ast->original_name)
				.c_str());
	}
	value = std::stoi(ast->token);
}

auto ASTIntegerConstant::build_ir(std::unique_ptr<bc::BuildContext>& bc) const
	-> llvm::Value*
{
	auto llvm_int_type = llvm::IntegerType::get(
		bc->context, std::numeric_limits<int>::digits + 1);

	return llvm::ConstantInt::getSigned(llvm_int_type, value);
}

void ASTIntegerConstant::get_dependencies(
	std::unordered_set<std::string>& functions,
	std::unordered_set<std::string>& types) const
{
	types.insert("Int");
}

} /* namespace by::ast */
