/*
 * FunctionType.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: Manuel Liebchen
 */

#include "function_type.h"

#include <cstddef> // for size_t
#include <utility>

#include <ext/alloc_traits.h>

#include <llvm/IR/DerivedTypes.h> // for FunctionType

#include "type_name.h"

namespace llvm {
class LLVMContext;
class Type;
} // namespace llvm

namespace by::type {

FunctionType::FunctionType(const std::shared_ptr<peg::Ast> &ast) {
  return_type = std::make_shared<const TypeName>(ast->nodes.back());
  for (size_t i = 0; i < ast->nodes.size() - 1; ++i) {
    parameters.emplace_back(std::make_shared<const TypeName>(ast->nodes[i]));
  }
}

FunctionType::FunctionType(TypeName_ptr returntype)
    : return_type(std::move(returntype)) {}

FunctionType::FunctionType(TypeName_ptr returntype, TypeName_ptr lhs)
    : return_type(std::move(returntype)) {
  parameters.push_back(std::move(lhs));
}
FunctionType::FunctionType(TypeName_ptr returntype, TypeName_ptr lhs,
                           TypeName_ptr rhs)
    : return_type(std::move(returntype)) {
  parameters.push_back(std::move(lhs));
  parameters.push_back(std::move(rhs));
}

FunctionType::FunctionType(TypeName_ptr return_type,
                           std::vector<TypeName_ptr> parameters)
    : return_type(std::move(return_type)), parameters(std::move(parameters)) {}

auto FunctionType::get_llvm_function_type(llvm::LLVMContext &context) const
    -> llvm::FunctionType * {
  std::vector<llvm::Type *> llvm_parameters;
  for (auto &para : parameters) {
    llvm_parameters.emplace_back(para->get_llvm_type(context));
  }
  llvm::Type *llvm_returntype = return_type->get_llvm_type(context);
  return llvm::FunctionType::get(llvm_returntype, llvm_parameters, false);
}

auto FunctionType::param_equal(const std::vector<TypeName_ptr> &rhs) const
    -> bool {
  if (parameters.size() != rhs.size()) {
    return false;
  }
  for (size_t i = 0; i < parameters.size(); ++i) {
    if (*parameters[i] != *rhs[i]) {
      return false;
    }
  }
  return true;
}

auto FunctionType::operator==(const FunctionType &rhs) const -> bool {
  if (return_type != rhs.return_type) {
    return false;
  }
  return param_equal(rhs.parameters);
}

auto FunctionType::operator!=(const FunctionType &rhs) const -> bool {
  return !(*this == rhs);
}

FunctionType::operator const TypeName &() const { return *return_type; }

} // namespace by::type

namespace std {

auto to_string(by::type::FunctionType const &func) -> std::string {
  std::string str = std::to_string(*func.return_type) + "(";
  if (!func.parameters.empty()) {
    str += std::to_string(func.parameters[0]);
    for (size_t i = 1; i < func.parameters.size(); ++i) {
      str += "," + std::to_string(func.parameters[i]);
    }
  }
  str += ")";
  return str;
}
} // namespace std
