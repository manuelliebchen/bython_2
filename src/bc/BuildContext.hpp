/*
 * BuildContext.hpp
 *
 *  Created on: Aug 8, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_BUILDCONTEXT_HPP_
#define SRC_AST_BUILDCONTEXT_HPP_

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include <stack>

#include "../type/FunctionType.hpp"
#include "../type/TypeName.hpp"

namespace by {
namespace ast {
struct ASTFunctionSignatur;
} // namespace ast

namespace bc {

struct compiling_error_exeption : std::exception {
	const std::shared_ptr<peg::Ast> ast;
	const std::string name;
	compiling_error_exeption(const std::shared_ptr<peg::Ast> ast,
							 const char* name)
		: ast(ast), name(std::string(name))
	{
	}
	[[nodiscard]] const char* what() const noexcept override
	{
		return (ast->path + ":" + std::to_string(ast->line) + ":" +
				std::to_string(ast->column) + ":" + name.c_str())
			.c_str();
	}
};

struct BuildContext {
	llvm::LLVMContext context;
	llvm::IRBuilder<> builder{context};
	llvm::Module module{"Bython Module", context};

	std::vector<std::unordered_map<std::string, llvm::Value*>> variables;

	type::function_map known_functions;
	std::unordered_set<type::TypeName> known_types;
};

} // namespace bc
} // namespace by

#endif /* SRC_AST_BUILDCONTEXT_HPP_ */
