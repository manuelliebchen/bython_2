/*
 * TypeName.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */
#include "type_name.h"

#include <algorithm>              // for max
#include <ext/alloc_traits.h>     // for __alloc_traits<>::value_type
#include <llvm/IR/DerivedTypes.h> // for PointerType, IntegerType
#include <llvm/IR/Type.h>         // for Type
#include <ostream>                // for operator<<, ostream, size_t
#include <stddef.h>               // for size_t
#include <stdexcept>              // for runtime_error
#include <string>                 // for allocator, operator+, string, oper...
#include <utility>                // for move

#include "../ast/expression.h"

namespace llvm {
class LLVMContext;
}

namespace by::type {

const std::shared_ptr<const TypeName> TypeName::Void =
    std::make_shared<const TypeName>("Void");
const std::shared_ptr<const TypeName> TypeName::Null =
    std::make_shared<const TypeName>("Null");
const std::shared_ptr<const TypeName> TypeName::None =
    std::make_shared<const TypeName>("None");
const std::shared_ptr<const TypeName> TypeName::Int =
    std::make_shared<const TypeName>("Int");
const std::shared_ptr<const TypeName> TypeName::Float =
    std::make_shared<const TypeName>("Float");
const std::shared_ptr<const TypeName> TypeName::String =
    std::make_shared<const TypeName>("String");
const std::shared_ptr<const TypeName> TypeName::List =
    std::make_shared<const TypeName>("List");

TypeName::TypeName(const std::shared_ptr<peg::Ast> &ast) {
  name = std::to_string(ast->nodes[0]);
  if (ast->nodes.size() > 1) {
    for (size_t i = 1; i < ast->nodes.size(); ++i) {
      if (ast->nodes[i]->original_name == "TypeName") {
        subtypes.push_back(TypeName(ast->nodes[i]));
      } else {
        break;
      }
    }
  }
}

TypeName::TypeName(std::string name, std::vector<TypeName> subtypes)
    : name(std::move(name)), subtypes(std::move(subtypes)) {}

TypeName::TypeName(std::string name) : name(std::move(name)), subtypes() {
  if (this->name == "") {
    this->name = "None";
  }
}

TypeName::TypeName(TypeName const &type) : name(type.name) {
  for (const auto &subtype : type.subtypes) {
    subtypes.push_back(TypeName(subtype));
  }
}

TypeName TypeName::operator=(TypeName type) {
  name = type.name;
  for (const auto &subtype : type.subtypes) {
    subtypes.push_back(TypeName(subtype));
  }
  return *this;
}

auto TypeName::deduct_type(TypeName rhs_name) const -> TypeName {
  if (*this == rhs_name) {
    return TypeName(*this);
  }
  if ((name == "Int" || name == "Float") &&
      (rhs_name.name == "Int" || rhs_name.name == "Float")) {
    return TypeName("Float");
  }
  throw std::runtime_error(
      "Type deduction failed with types: " + std::to_string(*this) + " and " +
      std::to_string(rhs_name));
}

llvm::Type *TypeName::get_llvm_type(llvm::LLVMContext &context) const {
  llvm::Type *type;
  if (name == "Int") {
    type = llvm::Type::getInt32Ty(context);
  } else if (name == "Void") {
    type = llvm::Type::getVoidTy(context);
  } else if (name == "Bool") {
    type = llvm::Type::getInt1Ty(context);
  } else if (name == "Float") {
    type = llvm::Type::getFloatTy(context);
  } else if (name == "String") {
    type = llvm::Type::getInt8PtrTy(context);
  } else if (name == "None") {
    throw std::runtime_error("Could not determin type!");
  } else {
    type = llvm::Type::getInt8PtrTy(context);
    //    type = llvm::PointerType::getUnqual(llvm::Type::getVoidTy(context));
  }
  return type;
}

bool TypeName::operator==(const TypeName &rhs) const {
  if (name != rhs.name) {
    return false;
  }
  if (subtypes.size() != rhs.subtypes.size()) {
    return false;
  }
  for (size_t i = 0; i < subtypes.size(); ++i) {
    if (subtypes[i] != rhs.subtypes[i]) {
      return false;
    }
  }

  return true;
}

bool TypeName::operator!=(const TypeName &rhs) const { return !(*this == rhs); }

auto TypeName::is_void() const -> bool { return *this == *Void; }
TypeName::operator bool() const { return !(*this == *None || *this == *Void); }

std::ostream &operator<<(std::ostream &os, const TypeName &type) {
  os << type.name;
  if (!type.subtypes.empty()) {
    os << "[";
    for (auto &subtype : type.subtypes) {
      os << subtype << ", ";
    }
    os << "]";
  }
  return os;
}

type_deduction_exeption::type_deduction_exeption(
    const std::shared_ptr<peg::Ast> ast, TypeName_ptr expected,
    TypeName_ptr got)
    : ast_error(
          ast, " error: type missmatch of types: " + std::to_string(*expected) +
                   " and " + std::to_string(*got)) {}

} // namespace by::type

namespace std {

std::string to_string(by::type::TypeName const &val) {
  std::string str = val.name;
  if (!val.subtypes.empty()) {
    str += "[";
    str += std::to_string(val.subtypes.front());
    for (size_t i = 1; i < val.subtypes.size(); ++i) {
      str += "," + std::to_string(val.subtypes[i]);
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
