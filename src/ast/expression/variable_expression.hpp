/*
 * ASTVariableExpression.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTVARIABLEEXPRESSION_HPP_
#define SRC_AST_ASTVARIABLEEXPRESSION_HPP_

#include <memory>

#include "../../ast/expression/expression.hpp"

namespace by::ast {

class ASTVariableExpression : public ASTExpression {
public:
  ASTVariableExpression(const std::shared_ptr<peg::Ast> &,
                        ASTBlockExpression *);

  by::type::TypeName determine_type(const type::function_map &);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

private:
  std::string name;
  std::shared_ptr<ASTVariableExpression> next;
  friend std::ostream &operator<<(std::ostream &,
                                  const ASTVariableExpression &);
};

inline std::ostream &operator<<(std::ostream &os,
                                const ASTVariableExpression &var) {
  os << var.name;
  if (var.next) {
    os << "." << *var.next;
  }
  return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_ASTVARIABLEEXPRESSION_HPP_ */
