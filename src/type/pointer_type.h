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
#include "llvm/IR/DerivedTypes.h"

#include "type_name.h"
namespace llvm {
class LLVMContext;
} // namespace llvm

namespace by::type {

struct PointerType : public TypeName {
  PointerType(const std::shared_ptr<peg::Ast> &ast);

  PointerType(const TypeName &);

  llvm::Type *get_llvm_type(llvm::LLVMContext &) const;

};

using PointerType_ptr = std::shared_ptr<const PointerType>;
using function_map = std::unordered_map<std::string, PointerType_ptr>;

} // namespace by::type
namespace std {
std::string to_string(by::type::PointerType const &);
} // namespace std

#endif /* SRC_AST_FUNCTIONTYPE_HPP_ */
