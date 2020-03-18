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

FunctionType::FunctionType(const std::shared_ptr<peg::Ast> &ast) {
  return_type = std::make_shared<const TypeName>(ast->nodes.back());
  for (size_t i = 0; i < ast->nodes.size() - 1; ++i) {
    parameters.emplace_back(std::make_shared<const TypeName>(ast->nodes[i]));
  }
}

FunctionType::FunctionType(const TypeName_ptr &returntype)
    : return_type(returntype) {}

FunctionType::FunctionType(const TypeName_ptr &returntype,
                           const TypeName_ptr &lhs)
    : return_type(returntype), parameters{lhs} {}
FunctionType::FunctionType(const TypeName_ptr &returntype,
                           const TypeName_ptr &lhs, const TypeName_ptr &rhs)
    : return_type(returntype), parameters{lhs, rhs} {}

FunctionType::FunctionType(const TypeName_ptr &return_type,
                           std::vector<TypeName_ptr> const &parameters)
    : return_type(return_type), parameters(parameters) {}

llvm::FunctionType *
FunctionType::get_llvm_function_type(llvm::LLVMContext &context) const {
  std::vector<llvm::Type *> llvm_parameters;
  for (auto &para : parameters) {
    llvm_parameters.emplace_back(para->get_llvm_type(context));
  }
  llvm::Type *llvm_returntype = return_type->get_llvm_type(context);
  return llvm::FunctionType::get(llvm_returntype, llvm_parameters, false);
}

bool FunctionType::operator==(const FunctionType &rhs) const {
  if (return_type != rhs.return_type) {
    return false;
  }
  if (parameters.size() != rhs.parameters.size()) {
    return false;
  }
  for (size_t i = 0; i < parameters.size(); ++i) {
    if (*parameters[i] != *rhs.parameters[i]) {
      return false;
    }
  }

  return true;
}

bool FunctionType::operator!=(const FunctionType &rhs) const {
  return !(*this == rhs);
}

} // namespace by::type

namespace std {

std::string to_string(by::type::FunctionType const &func) {
  std::string str = std::to_string(*(by::type::TypeName *)(&func)) + "(";
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
