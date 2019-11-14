/*
 * ASTCallExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "ASTCallExpression.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>

#include "utillib.hpp"

namespace by::ast {

ASTCallExpression::ASTCallExpression(const std::shared_ptr<peg::Ast>& ast)
	: ASTExpression(ast)
{
	if (ast->original_name != "CallExpression") {
		throw bad_ast_exeption(
			ast,
			(std::string("CallExpression but was ") + ast->original_name)
				.c_str());
	}
	name = util::to_string(ast->nodes[0]);
	for (size_t i = 1; i < ast->nodes.size(); ++i) {
		arguments.push_back(create_expression(ast->nodes[i]));
	}
}

llvm::Value*
ASTCallExpression::build_ir(std::unique_ptr<bc::BuildContext>& bc) const
{
	llvm::FunctionType* type =
		bc->known_functions.at(name).get_llvm_type(bc->context);
	llvm::Constant* function_callee =
		bc->module.getOrInsertFunction(name, type);

	std::vector<llvm::Value*> llvm_args;
	for (const auto& arg : arguments) {
		llvm_args.emplace_back(arg->build_ir(bc));
	}

	return bc->builder.CreateCall(type, function_callee, llvm_args);
}

void ASTCallExpression::get_dependencies(
	std::unordered_set<std::string>& functions,
	std::unordered_set<std::string>& types) const
{
	functions.insert(name);
	for (const auto& arg : arguments) {
		arg->get_dependencies(functions, types);
	}
}

} /* namespace by::ast */
