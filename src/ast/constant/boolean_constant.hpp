/*
 * ASTBooleanConstant.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_CONSTANT_BOOLEAN_CONSTANT_HPP_
#define SRC_AST_CONSTANT_BOOLEAN_CONSTANT_HPP_

#include "constant.hpp"

namespace by::ast {

class ASTBooleanConstant : public ASTConstant {
public:
  ASTBooleanConstant(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

private:
  bool value;

  friend std::ostream &operator<<(std::ostream &, const ASTBooleanConstant &);
};

inline std::ostream &operator<<(std::ostream &os,
                                const ASTBooleanConstant &boo) {
  if (boo.value) {
    os << "true";
  } else {
    os << "false";
  }
  return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_CONSTANT_BOOLEAN_CONSTANT_HPP_ */
