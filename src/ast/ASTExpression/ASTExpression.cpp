/*
 * ASTExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "ASTExpression.hpp"

#include <unordered_set>

#include "ASTArithmeticExpression.hpp"
#include "ASTBase.hpp"
#include "ASTBlockExpression.hpp"
#include "ASTCallExpression.hpp"
#include "ASTConstant.hpp"
#include "ASTIfExpression.hpp"
#include "ASTLetStatement.hpp"
#include "ASTNoneArithmeticExpression.hpp"
#include "ASTVariableExpression.hpp"

namespace by::ast {
ASTExpression::ASTExpression(const std::shared_ptr<peg::Ast>& ast,
							 ASTBlockExpression* parent)
	: ASTBase(ast, parent)
{
	//	std::cerr << "Constructiing Expression " << ast->original_name <<
	// std::endl;
}

auto ASTExpression::get_type() const -> by::type::TypeName
{
	return type;
}

auto operator<<(std::ostream& os, const by::ast::ASTExpression& exp)
	-> std::ostream&
{
	if (const auto* dyn =
			dynamic_cast<const ASTNoneArithmeticExpression*>(&exp)) {
		os << *dyn;
	}
	else if (const auto* dyn =
				 dynamic_cast<const ASTArithmeticExpression*>(&exp)) {
		os << *dyn;
	}
	else if (const auto* dyn = dynamic_cast<const ASTCallExpression*>(&exp)) {
		os << *dyn;
	}
	else if (const auto* dyn = dynamic_cast<const ASTIfExpression*>(&exp)) {
		os << *dyn;
	}
	else if (const auto* dyn = dynamic_cast<const ASTBlockExpression*>(&exp)) {
		os << *dyn;
	}
	else if (const auto* dyn = dynamic_cast<const ASTLetStatement*>(&exp)) {
		os << *dyn;
	}
	else if (const auto* dyn = dynamic_cast<const ASTConstant*>(&exp)) {
		os << *dyn;
	}
	else if (const auto* dyn =
				 dynamic_cast<const ASTVariableExpression*>(&exp)) {
		os << *dyn;
	}
	else {
		os << *dynamic_cast<const by::ast::ASTBase*>(&exp);
	}
	return os;
};

auto create_expression(const std::shared_ptr<peg::Ast>& ast,
					   ASTBlockExpression* parent)
	-> std::shared_ptr<by::ast::ASTExpression>
{
	const std::unordered_set<std::string> first_class_operator = {"*", "/", "%",
																  "&&"};
	const std::unordered_set<std::string> second_class_operator = {"+", "-",
																   "||"};
	const std::unordered_set<std::string> third_class_operator = {
		"!=", "==", ">=", "<=", ">", "<"};
	if (ast->original_name != "Expression") {
		throw bad_ast_exeption(
			ast,
			(std::string("Expression but was ") + ast->original_name).c_str());
	}

	if (ast->nodes.size() == 1) {
		return std::make_shared<by::ast::ASTNoneArithmeticExpression>(
			ast->nodes[0], parent);
	}

	std::shared_ptr<peg::Ast> ast_ptr = ast;
	std::vector<std::string> operators;
	std::vector<std::shared_ptr<ASTExpression>> expressions;

	operators.emplace_back(ast_ptr->nodes[1]->token);
	expressions.emplace_back(
		std::make_shared<by::ast::ASTNoneArithmeticExpression>(
			ast_ptr->nodes[0], parent));
	ast_ptr = ast_ptr->nodes[2];

	while (ast_ptr->nodes.size() > 1) {
		std::string bin_operator = ast_ptr->nodes[1]->token;
		if (first_class_operator.find(operators.back()) !=
			first_class_operator.end()) {
			expressions.back() = std::make_shared<ASTArithmeticExpression>(
				ast, parent, expressions.back(), operators.back(),
				std::make_shared<by::ast::ASTNoneArithmeticExpression>(
					ast_ptr->nodes[0], parent));
			operators.back() = bin_operator;
		}
		else {
			operators.emplace_back(ast_ptr->nodes[1]->token);
			expressions.emplace_back(
				std::make_shared<by::ast::ASTNoneArithmeticExpression>(
					ast_ptr->nodes[0], parent));
		}
		ast_ptr = ast_ptr->nodes[2];
	}

	if (first_class_operator.find(operators.back()) !=
		first_class_operator.end()) {
		expressions.back() = std::make_shared<ASTArithmeticExpression>(
			ast, parent, expressions.back(), operators.back(),
			std::make_shared<by::ast::ASTNoneArithmeticExpression>(
				ast_ptr->nodes[0], parent));
	}
	else {
		expressions.emplace_back(
			std::make_shared<by::ast::ASTNoneArithmeticExpression>(
				ast_ptr->nodes[0], parent));
	}

	for (size_t i = 0; i < operators.size(); ++i) {
		if (second_class_operator.find(operators[i]) !=
			second_class_operator.end()) {
			expressions[i] = std::make_shared<ASTArithmeticExpression>(
				ast, parent, expressions[i], operators[i], expressions[i + 1]);
			operators.erase(operators.begin() + i);
			expressions.erase(expressions.begin() + i + 1);
			i--;
		}
	}

	for (size_t i = 0; i < operators.size(); ++i) {
		if (third_class_operator.find(operators[i]) !=
			third_class_operator.end()) {
			expressions[i] = std::make_shared<ASTArithmeticExpression>(
				ast, parent, expressions[i], operators[i], expressions[i + 1]);
			operators.erase(operators.begin() + i);
			expressions.erase(expressions.begin() + i + 1);
			i--;
		}
	}

	return expressions.back();
}

} // namespace by::ast
