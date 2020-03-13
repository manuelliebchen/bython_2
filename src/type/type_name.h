/*
 * TypeName.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#ifndef SRC_AST_ASTTYPENAME_HPP_
#define SRC_AST_ASTTYPENAME_HPP_

#include <bits/exception.h> // for exception
#include <iosfwd>           // for ostream, size_t
#include <memory>           // for shared_ptr
#include <string>           // for string
#include <system_error>     // for hash
#include <unordered_map>    // for unordered_map
#include <unordered_set>    // for unordered_set
#include <vector>           // for vector

#include "../ast/ast_error.h"
#include "peglib.h" // for Ast

namespace llvm {
class LLVMContext;
}
namespace llvm {
class Type;
}

namespace by::type {
struct TypeName {
  std::string name;
  std::vector<TypeName> subtypes;

  TypeName(std::string, std::vector<TypeName>);
  TypeName(std::string);
  TypeName(TypeName const &);

  static const std::shared_ptr<const TypeName> Void;
  static const std::shared_ptr<const TypeName> Null;
  static const std::shared_ptr<const TypeName> None;
  static const std::shared_ptr<const TypeName> Int;
  static const std::shared_ptr<const TypeName> Float;
  static const std::shared_ptr<const TypeName> String;
  static const std::shared_ptr<const TypeName> List;
  static const std::shared_ptr<const TypeName> llist;

  TypeName(const std::shared_ptr<peg::Ast> &ast);
  TypeName operator=(TypeName);

  TypeName deduct_type(TypeName) const;

  bool operator==(const TypeName &) const;
  bool operator!=(const TypeName &) const;

  operator bool() const;
  bool is_void() const;

  llvm::Type *get_llvm_type(llvm::LLVMContext &) const;

  friend std::ostream &operator<<(std::ostream &, const TypeName &);
  friend std::string to_string(by::type::TypeName const &);
};

using TypeName_ptr = std::shared_ptr<const TypeName>;
using variable_map = std::unordered_map<std::string, TypeName_ptr>;
using type_set = std::unordered_set<TypeName_ptr>;

std::ostream &operator<<(std::ostream &, const TypeName &);

struct type_deduction_exeption : public by::ast::ast_error {
  type_deduction_exeption(const std::shared_ptr<peg::Ast> ast,
                          TypeName_ptr expected, TypeName_ptr got);
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
