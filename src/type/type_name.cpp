/*
 * TypeName.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */
#include "type_name.hpp"
#include <algorithm>              // for max
#include <ext/alloc_traits.h>     // for __alloc_traits<>::value_type
#include <llvm/IR/DerivedTypes.h> // for PointerType, IntegerType
#include <llvm/IR/Type.h>         // for Type
#include <ostream>                // for operator<<, ostream, size_t
#include <stddef.h>               // for size_t
#include <stdexcept>              // for runtime_error
#include <string>                 // for allocator, operator+, string, oper...
#include <util/util.hpp>          // for to_string
#include <utility>                // for move
namespace llvm {
class LLVMContext;
}

namespace by::type {

const std::shared_ptr<const TypeName> TypeName::Void =
    std::make_shared<const TypeName>("Void");
const std::shared_ptr<const TypeName> TypeName::None =
    std::make_shared<const TypeName>("None");

TypeName::TypeName() : name("None"), subtypes() {}

TypeName::TypeName(const std::shared_ptr<peg::Ast> &ast) {
  name = util::to_string(ast->nodes[0]);
  if (name.back() == '*') {
    generic = true;
    name = name.substr(0, name.length() - 1);
  }
  if (ast->nodes.size() > 1) {
    for (size_t i = 1; i < ast->nodes.size(); ++i) {
      subtypes.push_back(TypeName(ast->nodes[i]));
    }
  }
}

TypeName::TypeName(std::string name, std::vector<TypeName> subtypes)
    : name(std::move(name)), subtypes(std::move(subtypes)) {
  if (this->name.back() == '*') {
    generic = true;
    this->name = this->name.substr(0, name.length() - 1);
  }
}

TypeName::TypeName(std::string name) : name(std::move(name)), subtypes() {
  if (this->name == "") {
    this->name = "None";
  }
  if (this->name.back() == '*') {
    generic = true;
    this->name = this->name.substr(0, this->name.length() - 1);
  }
}

TypeName::TypeName(TypeName const &type)
    : name(type.name), generic(type.generic) {
  for (const auto &subtype : type.subtypes) {
    subtypes.push_back(TypeName(subtype));
  }
}

TypeName TypeName::operator=(TypeName type) {
  name = type.name;
  generic = type.generic;
  for (const auto &subtype : type.subtypes) {
    subtypes.push_back(TypeName(subtype));
  }
  return *this;
}

/// TODO
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
  if (generic) {
    return llvm::Type::getInt8PtrTy(context);
  } else if (name == "Int") {
    return llvm::Type::getInt32Ty(context);
  } else if (name == "Void") {
    return llvm::Type::getVoidTy(context);
  } else if (name == "Bool") {
    return llvm::Type::getInt1Ty(context);
  } else if (name == "Float") {
    return llvm::Type::getFloatTy(context);
  } else if (name == "List") {
    return llvm::Type::getInt8PtrTy(context);
  } else if (name == "String") {
    return llvm::Type::getInt8PtrTy(context);
  } else if (name == "None") {
    throw std::runtime_error("Could not determin type!");
  } else {
    throw std::runtime_error("Trying to get invalid llvm type: " +
                             std::to_string(*this));
  }
}

bool TypeName::operator==(const TypeName &rhs) const {
  if (name != rhs.name) {
    return false;
  }
  if (!(generic || rhs.generic)) {
    if (subtypes.size() != rhs.subtypes.size()) {
      return false;
    }
    for (size_t i = 0; i < subtypes.size(); ++i) {
      if (subtypes[i] != rhs.subtypes[i]) {
        return false;
      }
    }
  }
  return true;
}

bool TypeName::operator!=(const TypeName &rhs) const { return !(*this == rhs); }

auto TypeName::is_void() const -> bool { return *this == *Void; }
TypeName::operator bool() const { return !(*this == *None || *this == *Void); }

std::ostream &operator<<(std::ostream &os, const TypeName &type) {
  os << type.name;
  if (type.generic) {
    os << " *";
  }
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
    : ast(ast), expected(expected), got(got),
      what_str((ast->path + ":" + std::to_string(ast->line) + ":" +
                std::to_string(ast->column) +
                ": error: type missmatch of types: " +
                std::to_string(*expected) + " and " + std::to_string(*got))) {}

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
