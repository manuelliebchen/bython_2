/*
 * FunctionType.hpp
 *
 *  Created on: Aug 8, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_FUNCTIONTYPE_HPP_
#define SRC_AST_FUNCTIONTYPE_HPP_

#include <llvm/IR/Function.h>
#include <unordered_map>
#include <vector>

#include "TypeName.hpp"

namespace by::type {

struct FunctionType {
	FunctionType(const TypeName&);
	FunctionType(const TypeName&, std::vector<TypeName> const&);
	TypeName returntype;
	std::vector<TypeName> parameters;

	llvm::FunctionType* get_llvm_type(llvm::LLVMContext&) const;
};

using function_map = std::unordered_map<std::string, FunctionType>;

std::string to_string(by::type::FunctionType const&);
} // namespace by::type

#endif /* SRC_AST_FUNCTIONTYPE_HPP_ */
