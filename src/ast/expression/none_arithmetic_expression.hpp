/*
 * ASTNoneArithmeticExpression.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTNONEARITHMETICEXPRESSION_HPP_
#define SRC_AST_ASTNONEARITHMETICEXPRESSION_HPP_

#include "../../ast/expression/expression.hpp"

namespace by::ast {

class ASTNoneArithmeticExpression : public ASTExpression {
public:
  ASTNoneArithmeticExpression(const std::shared_ptr<peg::Ast> &,
                              ASTBlockExpression *);

  by::type::TypeName determine_type(const type::function_map &);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

private:
  std::string UnaryOperator;
  std::shared_ptr<ASTExpression> rhs;

  friend std::ostream &operator<<(std::ostream &,
                                  const ASTNoneArithmeticExpression &);
};

inline std::ostream &operator<<(std::ostream &os,
                                const ASTNoneArithmeticExpression &none) {
  os << none.UnaryOperator << *none.rhs;
  return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_ASTNONEARITHMETICEXPRESSION_HPP_ */
