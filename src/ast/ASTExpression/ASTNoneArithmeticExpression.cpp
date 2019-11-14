/*
 * ASTNoneArithmeticExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "ASTNoneArithmeticExpression.hpp"

#include "ASTBlockExpression.hpp"
#include "ASTBooleanConstant.hpp"
#include "ASTCallExpression.hpp"
#include "ASTFloatConstant.hpp"
#include "ASTIfExpression.hpp"
#include "ASTIntegerConstant.hpp"
#include "ASTLetStatement.hpp"
#include "ASTStringConstant.hpp"
#include "ASTVariableExpression.hpp"

namespace by::ast {

ASTNoneArithmeticExpression::ASTNoneArithmeticExpression(
	const std::shared_ptr<peg::Ast>& ast)
	: ASTExpression(ast)
{
	if (ast->original_name != "NoneArithmeticExpression") {
		throw bad_ast_exeption(
			ast,
			(std::string("NoneArithmeticExpression but was ") +
			 ast->original_name)
				.c_str());
	}

	size_t expression_node = 0;
	if (ast->nodes[0]->original_name == "UnaryOperator") {
		UnaryOperator = ast->nodes[0]->token;
		expression_node++;
	}
	else {
		UnaryOperator = "";
	}

	const std::shared_ptr<peg::Ast>& expression = ast->nodes[expression_node];
	if (expression->original_name == "Expression") {
		rhs = create_expression(expression);
	}
	else if (expression->original_name == "BlockExpression") {
		rhs = std::make_shared<ASTBlockExpression>(expression);
	}
	else if (expression->original_name == "CallExpression") {
		rhs = std::make_shared<ASTCallExpression>(expression);
	}
	else if (expression->original_name == "LetStatement") {
		rhs = std::make_shared<ASTLetStatement>(expression);
	}
	else if (expression->original_name == "IfExpression") {
		rhs = std::make_shared<ASTIfExpression>(expression);
	}
	else if (expression->original_name == "VariableExpression") {
		rhs = std::make_shared<ASTVariableExpression>(expression);
	}
	else if (expression->original_name == "FloatConstant") {
		rhs = std::make_shared<ASTFloatConstant>(expression);
	}
	else if (expression->original_name == "IntegerConstant") {
		rhs = std::make_shared<ASTIntegerConstant>(expression);
	}
	else if (expression->original_name == "BooleanConstant") {
		rhs = std::make_shared<ASTBooleanConstant>(expression);
	}
	else if (expression->original_name == "StringConstant") {
		rhs = std::make_shared<ASTStringConstant>(expression);
	}
	else {
		throw bad_ast_exeption(
			ast,
			(std::string("NoneArithmeticExpression subtype was ") +
			 expression->original_name)
				.c_str());
	}
}

llvm::Value* ASTNoneArithmeticExpression::build_ir(
	std::unique_ptr<bc::BuildContext>& bc) const
{
	llvm::Value* rhs_llvm = rhs->build_ir(bc);
	if (UnaryOperator == "-") {
		if (rhs_llvm->getType()->isFloatTy()) {
			return bc->builder.CreateFNeg(rhs_llvm);
		}
		if (rhs_llvm->getType()->isIntegerTy()) {
			return bc->builder.CreateNeg(rhs_llvm);
		}
	}
	if (UnaryOperator == "!") {
		return bc->builder.CreateNot(rhs_llvm);
	}
	if (UnaryOperator.empty()) {
		return rhs_llvm;
	}
	throw bc::compiling_error_exeption(ast, "Type not Implementet!");
}

void ASTNoneArithmeticExpression::get_dependencies(
	std::unordered_set<std::string>& functions,
	std::unordered_set<std::string>& types) const
{
	rhs->get_dependencies(functions, types);
}

} /* namespace by::ast */
