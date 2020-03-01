/*
 * FunctionType.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: Manuel Liebchen
 */

#include "function_type.h"

#include <cstddef>                // for size_t
#include <llvm/IR/DerivedTypes.h> // for FunctionType

#include "type_name.h"

namespace llvm {
class LLVMContext;
class Type;
} // namespace llvm

namespace by::type {

FunctionType::FunctionType(const std::shared_ptr<peg::Ast> &ast)
    : TypeName(ast->nodes.back()) {
  for (size_t i = 0; i < ast->nodes.size() - 1; ++i) {
    parameters.emplace_back(ast->nodes[i]);
  }
}

FunctionType::FunctionType(const TypeName &returntype) : TypeName(returntype) {}

FunctionType::FunctionType(const TypeName &returntype,
                           std::vector<TypeName> const &parameters)
    : TypeName(returntype), parameters(parameters) {}

llvm::FunctionType *
FunctionType::get_llvm_function_type(llvm::LLVMContext &context) const {
  std::vector<llvm::Type *> llvm_parameters;
  for (auto &para : parameters) {
    llvm_parameters.emplace_back(para.get_llvm_type(context));
  }
  llvm::Type *llvm_returntype = TypeName::get_llvm_type(context);
  return llvm::FunctionType::get(llvm_returntype, llvm_parameters, false);
}

BinaryOperator::BinaryOperator(const TypeName &type, const TypeName &lhs,
                               const TypeName &rhs)
    : TypeName(type), lhs(lhs), rhs(rhs) {}

auto BinaryOperator::get_llvm_function_type(llvm::LLVMContext &context) const
    -> llvm::FunctionType * {
  std::vector<llvm::Type *> llvm_parameters{lhs.get_llvm_type(context),
                                            rhs.get_llvm_type(context)};
  llvm::Type *llvm_returntype = TypeName::get_llvm_type(context);
  return llvm::FunctionType::get(llvm_returntype, llvm_parameters, false);
}
} // namespace by::type

namespace std {

std::string to_string(by::type::FunctionType const &func) {
  std::string str = std::to_string(static_cast<by::type::TypeName>(func)) + "(";
  if (!func.parameters.empty()) {
    str += std::to_string(func.parameters[0]);
    for (size_t i = 1; i < func.parameters.size(); ++i) {
      str += "," + std::to_string(func.parameters[i]);
    }
  }
  str += ")";
  return str;
}

std::string to_string(by::type::BinaryOperator const &func) {
  std::string str = std::to_string(static_cast<by::type::TypeName>(func)) + "(";
  str += std::to_string(func.lhs) + "," + std::to_string(func.rhs) + ")";
  return str;
}
} // namespace std
