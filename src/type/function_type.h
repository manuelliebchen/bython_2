/*
 * FunctionType.hpp
 *
 *  Created on: Aug 8, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_FUNCTIONTYPE_HPP_
#define SRC_AST_FUNCTIONTYPE_HPP_

#include <unordered_map>
#include <vector>

#include "peglib.h" // for Ast
#include <memory> // for shared_ptr
#include <string> // for string

#include "type_name.h"
namespace llvm {
class FunctionType;
class LLVMContext;
} // namespace llvm

namespace by::type {

	struct FunctionType : public TypeName {
  std::vector<TypeName> parameters;

  FunctionType(const TypeName &);
  FunctionType(const TypeName &, std::vector<TypeName> const &);

  FunctionType(const std::shared_ptr<peg::Ast> &ast);

  llvm::FunctionType *get_llvm_function_type(llvm::LLVMContext &) const;

  friend std::string to_string(by::type::FunctionType const &);
};

using FunctionType_ptr = std::shared_ptr<const FunctionType>;
using function_map = std::unordered_map<std::string, FunctionType_ptr>;

struct BinaryOperator : public TypeName {
  TypeName lhs;
  TypeName rhs;
  BinaryOperator(const TypeName &, const TypeName &, const TypeName &);

  llvm::FunctionType *get_llvm_function_type(llvm::LLVMContext &) const;

  friend std::string to_string(by::type::BinaryOperator const &);
};

using BinaryOperator_ptr = std::shared_ptr<const BinaryOperator>;
using binary_operator_map = std::unordered_map<std::string, BinaryOperator_ptr>;

} // namespace by::type
namespace std {
std::string to_string(by::type::FunctionType const &);
std::string to_string(by::type::BinaryOperator const &);
} // namespace std

#endif /* SRC_AST_FUNCTIONTYPE_HPP_ */