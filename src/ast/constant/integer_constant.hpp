/*
 * ASTIntegerConstant.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTINTEGERCONSTANT_HPP_
#define SRC_AST_ASTINTEGERCONSTANT_HPP_

#include "../constant/constant.hpp"

namespace by::ast {

class ASTIntegerConstant : public ASTConstant {
public:
  ASTIntegerConstant(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

private:
  int value;

  friend std::ostream &operator<<(std::ostream &, const ASTIntegerConstant &);
};

inline std::ostream &operator<<(std::ostream &os,
                                const ASTIntegerConstant &in) {
  os << std::to_string(in.value);
  return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_ASTINTEGERCONSTANT_HPP_ */
