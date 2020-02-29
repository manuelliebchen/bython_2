/*
 * ASTArithmeticExpression.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: jemand
 */

#ifndef SRC_AST_ASTARITHMETICEXPRESSION_HPP_
#define SRC_AST_ASTARITHMETICEXPRESSION_HPP_

#include <map>
#include <memory>

#include <peglib.h>

#include "expression.hpp"

namespace by::ast {

class ASTArithmeticExpression : public ASTExpression {
private:
  std::shared_ptr<ASTExpression> lhs;
  std::string BinaryOperator;
  std::shared_ptr<ASTExpression> rhs;

  type::TypeName_ptr operation_type;

public:
  ASTArithmeticExpression(const std::shared_ptr<peg::Ast> &,
                          ASTBlockExpression *, std::shared_ptr<ASTExpression>,
                          std::string, std::shared_ptr<ASTExpression>);

  by::type::TypeName_ptr determine_type(type::variable_map &);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

  static const std::multimap<std::string, type::BinaryOperator> operators;

  friend std::ostream &operator<<(std::ostream &,
                                  const ASTArithmeticExpression &);
};

inline std::ostream &operator<<(std::ostream &os,
                                const ASTArithmeticExpression &arit) {
  os << *arit.lhs << " " << arit.BinaryOperator << " " << *arit.rhs;
  return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_ASTARITHMETICEXPRESSION_HPP_ */
