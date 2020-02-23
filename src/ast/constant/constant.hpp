/*
 * ASTConstant.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTCONSTANT_HPP_
#define SRC_AST_ASTCONSTANT_HPP_

#include "../expression/expression.hpp"

namespace by::ast {

class ASTConstant : public ASTExpression {
public:
  ASTConstant(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

private:
  friend std::ostream &operator<<(std::ostream &, const ASTConstant &);
};

std::ostream &operator<<(std::ostream &, const ASTConstant &);

} /* namespace by::ast */

#endif /* SRC_AST_ASTCONSTANT_HPP_ */
