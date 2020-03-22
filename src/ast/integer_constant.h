/*
 * ASTIntegerConstant.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTINTEGERCONSTANT_HPP_
#define SRC_AST_ASTINTEGERCONSTANT_HPP_

#include <llvm/IR/Value.h>
#include <iosfwd> // for ostream
#include <memory> // for shared_ptr, unique_ptr
#include <string> // for operator<<, to_string

#include "constant.h" // for ASTConstant
#include "peglib.h"     // for Ast

namespace by::ast {
class ASTBlockExpression;
}
namespace by::bc {
	class BuildContext;
}
namespace llvm {
class Value;
}

namespace by::ast {

class ASTIntegerConstant : public ASTConstant {
public:
  ASTIntegerConstant(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

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
