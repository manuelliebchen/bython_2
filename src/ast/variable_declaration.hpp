/*
 * VariableDeclaration.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#ifndef SRC_AST_VARIABLE_DECLARATION_HPP_
#define SRC_AST_VARIABLE_DECLARATION_HPP_

#include <memory>

#include "../type/type_name.hpp"
#include "base.hpp"
#include "expression/block_expression.hpp"
#include "peglib.h"

namespace by::ast {

class ASTVariableDeclaration : public ASTBase {
protected:
  std::string name;
  by::type::TypeName_ptr type;

public:
  ASTVariableDeclaration(const std::shared_ptr<peg::Ast> &,
                         ASTBlockExpression *);

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

  const std::string &get_name() const;
  by::type::TypeName_ptr get_type() const;
  friend std::ostream &operator<<(std::ostream &,
                                  const ASTVariableDeclaration &);
};

inline std::ostream &operator<<(std::ostream &os,
                                const ASTVariableDeclaration &var) {
  os << var.name << " : " << *var.type;
  return os;
}

} // namespace by::ast

#endif /* SRC_AST_VARIABLE_DECLARATION_HPP_ */
