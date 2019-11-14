/*
 * ASTRoot.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "ASTRoot.hpp"

namespace by::ast {

ASTRoot::ASTRoot(const std::shared_ptr<peg::Ast>& ast) : ASTBase(ast)
{
	for (const auto& node : ast->nodes) {
		functions.push_back(std::make_shared<ASTFunction>(node));
	}
}

void ASTRoot::get_dependencies(std::unordered_set<std::string>& functions,
							   std::unordered_set<std::string>& types) const
{
	for (const auto& func : this->functions) {
		func->get_dependencies(functions, types);
	}
}

const std::vector<std::shared_ptr<by::ast::ASTFunction>>&
ASTRoot::get_functions()
{
	return functions;
}

std::ostream& operator<<(std::ostream& os, const ASTRoot& root)
{
	for (auto& function : root.functions) {
		os << *function << std::endl;
	}
	return os;
};

} /* namespace by::ast */
