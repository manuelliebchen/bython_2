/*
 * TypeName.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */
#include "type_name.h"

#include <cstddef>   // for size_t
#include <ostream>   // for operator<<, ostream, size_t
#include <stdexcept> // for runtime_error
#include <string>    // for allocator, operator+, string, oper...
#include <utility>   // for move

#include <ext/alloc_traits.h> // for __alloc_traits<>::value_type

#include <llvm/IR/DerivedTypes.h> // for PointerType, IntegerType
#include <llvm/IR/Type.h>         // for Type

#include "../ast/expression.h"
#include "ast/ast_error.h"

namespace llvm {
class LLVMContext;
} // namespace llvm

namespace by::type {

const TypeName_ptr TypeName::Null = std::make_shared<const TypeName>("Null");
const TypeName_ptr TypeName::Void = std::make_shared<const TypeName>("Void");
const TypeName_ptr TypeName::None = std::make_shared<const TypeName>("None");
const TypeName_ptr TypeName::Bool = std::make_shared<const TypeName>("Bool");
const TypeName_ptr TypeName::Int = std::make_shared<const TypeName>("Int");
const TypeName_ptr TypeName::Float = std::make_shared<const TypeName>("Float");

const TypeName_ptr TypeName::List = std::make_shared<const TypeName>("List");
const TypeName_ptr TypeName::String =
    std::make_shared<const TypeName>("String");

const std::vector<TypeName_ptr> TypeName::native = {
    TypeName::Bool, TypeName::Int, TypeName::Float};

TypeName::TypeName(const std::shared_ptr<peg::Ast> &ast) {
  name = std::to_string(ast->nodes[0]);
  if (ast->nodes.size() > 1) {
    for (size_t i = 1; i < ast->nodes.size(); ++i) {
      if (ast->nodes[i]->original_name == "TypeName") {
        subtypes.push_back(std::make_shared<const TypeName>(ast->nodes[i]));
      } else {
        break;
      }
    }
  }
}

TypeName::TypeName(std::string name, std::vector<TypeName_ptr> subtypes)
    : name(std::move(name)), subtypes(std::move(subtypes)) {}

TypeName::TypeName(std::string name) : name(std::move(name)) {
  if (this->name.empty()) {
    this->name = "None";
  }
}

TypeName::TypeName(TypeName const &type) : name(type.name) {
  for (const auto &subtype : type.subtypes) {
    subtypes.push_back(std::make_shared<const TypeName>(*subtype));
  }
}

auto TypeName::make(const TypeName &type) -> std::shared_ptr<const TypeName> {
  return std::make_shared<const TypeName>(type);
}

auto TypeName::operator=(const TypeName &type) -> TypeName & {
  name = type.name;
  for (const auto &subtype : type.subtypes) {
    subtypes.push_back(std::make_shared<const TypeName>(*subtype));
  }
  return *this;
}

auto TypeName::deduct_type(const TypeName &rhs_name) const -> TypeName {
  if (*this == rhs_name) {
    return TypeName(*this);
  }
  if ((name == "Int" || name == "Float") &&
      (rhs_name.name == "Int" || rhs_name.name == "Float")) {
    return TypeName("Float");
  }
  if ((!this->is_native() && !rhs_name.is_native())) {
    if (*this == *Null) {
      return rhs_name;
    }
    if (rhs_name == *Null) {
      return *this;
    }
  }
  throw std::runtime_error(
      "Type deduction failed with types: " + std::to_string(*this) + " and " +
      std::to_string(rhs_name));
}

auto TypeName::get_llvm_type(llvm::LLVMContext &context) const -> llvm::Type * {
  if (name == "Void") {
    return llvm::Type::getVoidTy(context);
  }
  if (name == "Bool") {
    return llvm::Type::getInt1Ty(context);
  }
  if (name == "Int") {
    return llvm::Type::getInt32Ty(context);
  }
  if (name == "Float") {
    return llvm::Type::getFloatTy(context);
  }
  if (name == "None") {
    throw std::runtime_error("Could not determin type!");
  }
  return llvm::Type::getInt8PtrTy(context);
}

auto TypeName::operator==(const TypeName &rhs) const -> bool {
  if (name != rhs.name) {
    return false;
  }
  if (subtypes.size() != rhs.subtypes.size()) {
    return false;
  }
  for (size_t i = 0; i < subtypes.size(); ++i) {
    if (*subtypes[i] != *rhs.subtypes[i]) {
      return false;
    }
  }

  return true;
}

auto TypeName::operator!=(const TypeName &rhs) const -> bool {
  return !(*this == rhs);
}

TypeName::operator bool() const { return !(*this == *None || *this == *Void); }
auto TypeName::is_void() const -> bool { return *this == *Void; }
auto TypeName::is_native() const -> bool {
  for (auto &type : TypeName::native) {
    if (*type == *this) {
      return true;
    }
  }
  return false;
}

auto operator<<(std::ostream &os, const TypeName &type) -> std::ostream & {
  os << type.name;
  if (!type.subtypes.empty()) {
    os << "[";
    for (auto &subtype : type.subtypes) {
      os << *subtype << ", ";
    }
    os << "]";
  }
  return os;
}

type_deduction_exeption::type_deduction_exeption(
    const std::shared_ptr<peg::Ast> &ast, const TypeName_ptr &expected,
    const TypeName_ptr &got)
    : ast_error(
          ast, " error: type missmatch of types: " + std::to_string(*expected) +
                   " and " + std::to_string(*got)) {}

} // namespace by::type

namespace std {

auto to_string(by::type::TypeName const &val) -> std::string {
  std::string str = val.name;
  if (!val.subtypes.empty()) {
    str += "[";
    str += std::to_string(*val.subtypes.front());
    for (size_t i = 1; i < val.subtypes.size(); ++i) {
      str += "," + std::to_string(*val.subtypes[i]);
    }
    str += "]";
  }
  return str;
}

auto std::hash<by::type::TypeName>::operator()(argument_type const &s) const
    noexcept -> result_type {
  return std::hash<std::string>{}(std::to_string(s));
}
} // namespace std
