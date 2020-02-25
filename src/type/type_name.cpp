/*
 * TypeName.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */
#include "type_name.hpp"

#include <llvm/IR/Constants.h>
#include <string>

#include "../utillib/utillib.hpp"

namespace by::type {

TypeName::TypeName() : name("None"), subtypes() {}

TypeName::TypeName(const std::shared_ptr<peg::Ast> &ast) {
  if (ast->original_name != "TypeName") {
    throw ast::bad_ast_exeption(
        ast, (std::string("TypeName but was ") + ast->original_name).c_str());
  }
  name = util::to_string(ast->nodes[0]);
  if (ast->nodes.size() > 1) {
    for (size_t i = 1; i < ast->nodes.size(); ++i) {
      subtypes.push_back(TypeName(ast->nodes[i]));
    }
  }
}

TypeName::TypeName(std::string name, std::vector<TypeName> subtypes)
    : name(std::move(name)), subtypes(std::move(subtypes)) {}

TypeName::TypeName(std::string name) : name(std::move(name)), subtypes() {}
// TypeName::TypeName(const char* name) : name(std::string(name)){};

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

/// TODO
auto TypeName::deduct_type(TypeName) const -> TypeName {
  return TypeName(name);
}

llvm::Type *TypeName::get_llvm_type(llvm::LLVMContext &context) const {
  if (name == "Int") {
    return llvm::Type::getInt32Ty(context);
  } else if (name == "Void") {
    return llvm::Type::getVoidTy(context);
  } else if (name == "Bool") {
    return llvm::ConstantInt::getTrue(context)->getType();
  } else if (name == "Float") {
    return llvm::Type::getFloatTy(context);
  } else if (name == "List") {
    return llvm::Type::getInt8PtrTy(context);
  } else if (name == "List*") {
    return llvm::Type::getInt8PtrTy(context);
  } else if (name == "String") {
    return llvm::Type::getInt8PtrTy(context);
  } else if (name.back() == '*') {
    return llvm::Type::getInt8PtrTy(context);
  } else if (name == "None") {
    throw std::runtime_error("Could not determin type!");
  } else {
    throw std::runtime_error("Trying to convert invalid type: " + name);
  }
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

TypeName::operator bool() const { return name != "None"; }

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

std::string to_string(TypeName const &val) {
  std::string str = val.name;
  if (!val.subtypes.empty()) {
    str += "[";
    str += type::to_string(val.subtypes.front());
    for (size_t i = 1; i < val.subtypes.size(); ++i) {
      str += "," + to_string(val.subtypes[i]);
    }
    str += "]";
  }
  return str;
}

} // namespace by::type

namespace std {

auto std::hash<by::type::TypeName>::operator()(argument_type const &s) const
    noexcept -> result_type {
  return std::hash<std::string>{}(by::type::to_string(s));
}
} // namespace std
