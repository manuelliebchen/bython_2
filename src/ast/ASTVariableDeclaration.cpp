/*
 * VariableDeclaration.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#include "ASTVariableDeclaration.hpp"

#include "../utillib/utillib.hpp"
namespace by::ast {

ASTVariableDeclaration::ASTVariableDeclaration(
	const std::shared_ptr<peg::Ast>& ast)
	: ASTBase(ast)
{
	if (ast->original_name != "VariableDeclaration") {
		throw bad_ast_exeption(
			ast,
			(std::string("VariableDeclaration but was ") + ast->original_name)
				.c_str());
	}
	name = util::to_string(ast->nodes[0]);
	type = std::make_shared<by::type::TypeName>(ast->nodes[1]);
}

void ASTVariableDeclaration::get_dependencies(
	std::unordered_set<std::string>& functions,
	std::unordered_set<std::string>& types) const
{
	types.insert(type::to_string(*type));
}

const std::string& ASTVariableDeclaration::get_name() const
{
	return name;
}

const std::shared_ptr<by::type::TypeName>&
ASTVariableDeclaration::get_type() const
{
	return type;
}

} // namespace by::ast
