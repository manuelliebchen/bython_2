/*
 * VariableDeclaration.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#ifndef SRC_AST_VARIABLE_DECLARATION_H_
#define SRC_AST_VARIABLE_DECLARATION_H_

#include <llvm/IR/Value.h>
#include <memory>
#include <ostream>
#include <peglib.h>
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
class ASTVariableDeclaration : public ASTExpression {
protected:
  /**
   * Name of the variable
   */
  std::string name;

public:
  ASTVariableDeclaration(const std::shared_ptr<peg::Ast> &,
                         ASTBlockExpression *);

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

  const std::string &get_name() const;

  by::type::TypeName_ptr determine_type(type::variable_map &);

  friend std::ostream &operator<<(std::ostream &,
                                  const ASTVariableDeclaration &);
};

inline std::ostream &operator<<(std::ostream &os,
                                const ASTVariableDeclaration &var) {
  os << var.name << " : " << *var.type;
  return os;
}

} // namespace by::ast

#endif /* SRC_AST_VARIABLE_DECLARATION_H_ */
