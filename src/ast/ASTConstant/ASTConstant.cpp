/*
 * ASTConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "ASTConstant.hpp"

#include "ASTBooleanConstant.hpp"
#include "ASTFloatConstant.hpp"
#include "ASTIntegerConstant.hpp"
#include "ASTStringConstant.hpp"

namespace by::ast {

ASTConstant::ASTConstant(const std::shared_ptr<peg::Ast>& ast)
	: ASTExpression(ast)
{
}

std::ostream& operator<<(std::ostream& os, const ASTConstant& constant)
{
	if (const auto* dyn = dynamic_cast<const ASTIntegerConstant*>(&constant)) {
		os << *dyn;
	}
	else if (const auto* dyn =
				 dynamic_cast<const ASTFloatConstant*>(&constant)) {
		os << *dyn;
	}
	else if (const auto* dyn =
				 dynamic_cast<const ASTBooleanConstant*>(&constant)) {
		os << *dyn;
	}
	else if (const auto* dyn =
				 dynamic_cast<const ASTStringConstant*>(&constant)) {
		os << *dyn;
	}
	else {
		os << *dynamic_cast<const ASTExpression*>(&constant);
	}
	return os;
}

} /* namespace by::ast */
