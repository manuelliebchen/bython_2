/*
 * TypeName.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#ifndef SRC_AST_ASTTYPENAME_HPP_
#define SRC_AST_ASTTYPENAME_HPP_

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <unordered_map>

#include "../ast/base.hpp"

namespace by::type {
struct TypeName {

  std::string name;
  std::vector<TypeName> subtypes;

  TypeName();
  TypeName(const std::shared_ptr<peg::Ast> &ast);
  TypeName(std::string, std::vector<TypeName>);
  TypeName(std::string);
  TypeName(TypeName const &);

  TypeName operator=(TypeName);

  TypeName deduct_type(TypeName) const;

  bool operator==(const TypeName &) const;
  bool operator!=(const TypeName &) const;

  operator bool() const;
  bool is_void() const;

  llvm::Type *get_llvm_type(llvm::LLVMContext &) const;

  static const std::shared_ptr<const TypeName> Void;
  static const std::shared_ptr<const TypeName> None;

  friend std::ostream &operator<<(std::ostream &, const TypeName &);
  friend std::string to_string(by::type::TypeName const &);
};

using TypeName_ptr = std::shared_ptr<const TypeName>;
using variable_map = std::unordered_map<std::string, TypeName_ptr>;

std::ostream &operator<<(std::ostream &, const TypeName &);

struct type_deduction_exeption : std::exception {
  const std::shared_ptr<peg::Ast> ast;
  TypeName_ptr expected;
  TypeName_ptr got;
  const std::string what_str;

  type_deduction_exeption(const std::shared_ptr<peg::Ast> ast,
                          TypeName_ptr expected, TypeName_ptr got);

  [[nodiscard]] const char *what() const noexcept override {
    return what_str.c_str();
  }
};
} // namespace by::type

namespace std {
std::string to_string(by::type::TypeName const &);
template <> struct hash<by::type::TypeName> {
  typedef by::type::TypeName argument_type;
  typedef std::size_t result_type;
  result_type operator()(argument_type const &) const noexcept;
};
} // namespace std

#endif /* SRC_AST_TypeName_HPP_ */
