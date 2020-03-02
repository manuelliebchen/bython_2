/*
 * FunctionType.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: Manuel Liebchen
 */

#include "pointer_type.h"

#include <cstddef>                // for size_t
#include <llvm/IR/DerivedTypes.h> // for FunctionType

#include "type_name.h"

namespace llvm {
class LLVMContext;
class Type;
} // namespace llvm

namespace by::type {

PointerType::PointerType(const std::shared_ptr<peg::Ast> &ast)
    : TypeName(ast->nodes.back()) {}

PointerType::PointerType(const TypeName &returntype) : TypeName(returntype) {}

llvm::Type *PointerType::get_llvm_type(llvm::LLVMContext &context) const {
  return TypeName::get_llvm_type(context)->getPointerTo();
}

} // namespace by::type

namespace std {

std::string to_string(by::type::PointerType const &func) {
  std::string str = std::to_string(static_cast<by::type::TypeName>(func)) + "*";
  return str;
}
} // namespace std
