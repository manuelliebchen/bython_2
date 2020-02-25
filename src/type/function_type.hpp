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

#include "type_name.hpp"

namespace by::type {

struct FunctionType {
  TypeName returntype;
  std::vector<TypeName> parameters;

  FunctionType(const TypeName &);
  FunctionType(const TypeName &, std::vector<TypeName> const &);

  llvm::FunctionType *get_llvm_type(llvm::LLVMContext &) const;
};

using function_map = std::unordered_map<std::string, FunctionType>;
} // namespace by::type
namespace std {
std::string to_string(by::type::FunctionType const &);
} // namespace std

#endif /* SRC_AST_FUNCTIONTYPE_HPP_ */
