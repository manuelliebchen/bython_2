/*
 * FunctionType.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: Manuel Liebchen
 */

#include "FunctionType.hpp"

namespace by::type {

FunctionType::FunctionType(const TypeName& returntype) : returntype(returntype)
{
}

FunctionType::FunctionType(const TypeName& returntype,
						   std::vector<TypeName> const& parameters)
	: returntype(returntype), parameters(parameters)
{
}

llvm::FunctionType*
FunctionType::get_llvm_type(llvm::LLVMContext& context) const
{
	std::vector<llvm::Type*> llvm_parameters;
	for (auto& para : parameters) {
		llvm_parameters.emplace_back(para.get_llvm_type(context));
	}
	llvm::Type* llvm_returntype = returntype.get_llvm_type(context);
	return llvm::FunctionType::get(llvm_returntype, llvm_parameters, false);
}

std::string to_string(FunctionType const& func)
{
	std::string str = to_string(func.returntype) + "(";
	if (!func.parameters.empty()) {
		str += to_string(func.parameters[0]);
		for (size_t i = 1; i < func.parameters.size(); ++i) {
			str += "," + to_string(func.parameters[i]);
		}
	}
	str += ")";
	return str;
}
} // namespace by::type
