/*
 * VariableDeclaration.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#ifndef SRC_AST_VARIABLE_DECLARATION_H_
#define SRC_AST_VARIABLE_DECLARATION_H_

#include <llvm/IR/Value.h>
#include <peglib.h>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>

#include "../type/type_name.h"
#include "block_expression.h"
#include "expression.h"

namespace by {
namespace bc {
class BuildContext;
} // namespace bc

namespace ast {
class ASTBlockExpression;
} // namespace ast
} // namespace by

namespace by::ast {

/**
 * Simple AST node to save name,type relations
 */
class ASTVariableDeclaration {
protected:
  /**
   * Name of the variable
   */
  std::string name;
  /**
   * Pointer to peg::Ast object of this AST node
   */
  const std::shared_ptr<peg::Ast> ast;

  /**
   * type of the expression
   */
  type::TypeName_ptr type;

public:
  ASTVariableDeclaration(const std::shared_ptr<peg::Ast> &);

  const std::string &get_name() const;

  /**
   * return type of expression
   */
  by::type::TypeName_ptr get_type() const;

  friend std::ostream &operator<<(std::ostream &,
                                  const ASTVariableDeclaration &);
};
std::ostream &operator<<(std::ostream &,
                                  const ASTVariableDeclaration &);

} // namespace by::ast

#endif /* SRC_AST_VARIABLE_DECLARATION_H_ */
