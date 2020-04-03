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

struct FunctionType {
  TypeName_ptr return_type;
  std::vector<TypeName_ptr> parameters;

  FunctionType(const std::shared_ptr<peg::Ast> &ast);


  FunctionType();
  FunctionType(TypeName_ptr);
  FunctionType(TypeName_ptr, TypeName_ptr);
  FunctionType(TypeName_ptr, TypeName_ptr, TypeName_ptr);
  FunctionType(TypeName_ptr, std::vector<TypeName_ptr>);

  TypeName_ptr lhs() const { return parameters[0];}
  TypeName_ptr rhs() const { return parameters[1];}

  llvm::FunctionType *get_llvm_function_type(llvm::LLVMContext &) const;

  bool param_equal(const std::vector<TypeName_ptr>&) const;
  bool operator== (const FunctionType&) const;
  bool operator!= (const FunctionType&) const;

  operator const TypeName&() const;

  friend std::string to_string(by::type::FunctionType const &);

  friend std::ostream &operator<<(std::ostream &, const FunctionType &);
};

std::ostream &operator<<(std::ostream &, const FunctionType &);

using FunctionType_ptr = std::shared_ptr<const FunctionType>;
using function_map = std::unordered_map<std::string, FunctionType_ptr>;

} // namespace by::type
namespace std {
std::string to_string(by::type::FunctionType const &);
} // namespace std

#endif /* SRC_AST_FUNCTIONTYPE_HPP_ */
