/*
 * ASTArithmeticExpression.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: jemand
 */

#ifndef SRC_AST_ASTARITHMETICEXPRESSION_HPP_
#define SRC_AST_ASTARITHMETICEXPRESSION_HPP_

#include <llvm/IR/Value.h>
#include <map>
#include <memory>
#include <ostream>
#include <peglib.h>
#include <string>
#include <unordered_set>

#include "../type/type_name.h"
#include "expression.h"

namespace by {
namespace ast {
class ASTBlockExpression;
} // namespace ast
namespace bc {
struct BuildContext;
} // namespace bc
namespace type {
struct BinaryOperator;
} // namespace type
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {

/**
 * AST node for binary expressions
 */
class ASTArithmeticExpression : public ASTExpression {
private:
  /**
   * left hand side of the exprssion
   */
  std::shared_ptr<ASTExpression> lhs;
  /**
   * Binary Operator
   */
  std::string BinaryOperator;
  /**
   * right hand side of the expression
   */
  std::shared_ptr<ASTExpression> rhs;

  /**
   * type of the sub expression
   */
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