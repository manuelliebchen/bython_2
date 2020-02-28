/*
 * ASTStringConstant.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_CONSTANT_STRING_CONSTANT_HPP_
#define SRC_AST_CONSTANT_STRING_CONSTANT_HPP_

#include <string>

#include "../../ast/constant/constant.hpp"

namespace by::ast {

class ASTStringConstant : public ASTConstant {
public:
  ASTStringConstant(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

private:
  std::string value;

  friend std::ostream &operator<<(std::ostream &, const ASTStringConstant &);
};

inline std::ostream &operator<<(std::ostream &os,
                                const ASTStringConstant &str) {
  os << "\"" << str.value << "\"";
  return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_CONSTANT_STRING_CONSTANT_HPP_ */
