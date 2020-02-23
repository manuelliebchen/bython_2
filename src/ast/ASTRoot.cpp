/*
 * ASTRoot.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "ASTRoot.hpp"

namespace by::ast {

ASTRoot::ASTRoot(const std::shared_ptr<peg::Ast>& ast,
				 ASTBlockExpression* parent)
	: ASTBase(ast, parent)
{
	for (const auto& node : ast->nodes) {
		functions.push_back(std::make_shared<ASTFunction>(node, parent));
	}
}

void ASTRoot::get_dependencies(std::unordered_set<std::string>& functions,
							   std::unordered_set<std::string>& types) const
{
	for (const auto& func : this->functions) {
		func->get_dependencies(functions, types);
	}
}

auto ASTRoot::get_functions()
	-> const std::vector<std::shared_ptr<by::ast::ASTFunction>>&
{
	return functions;
}

auto operator<<(std::ostream& os, const ASTRoot& root) -> std::ostream&
{
	for (auto& function : root.functions) {
		os << *function << std::endl;
	}
	return os;
};

} /* namespace by::ast */
