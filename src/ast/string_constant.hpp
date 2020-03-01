/*
 * ASTStringConstant.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_STRING_CONSTANT_HPP_
#define SRC_AST_STRING_CONSTANT_HPP_

#include <llvm/IR/Value.h>
#include <memory>
#include <ostream>
#include <string>

#include "constant.hpp"
#include "peglib.h"

namespace by {
namespace ast {
class ASTBlockExpression;
} // namespace ast
namespace bc {
struct BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

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

#endif /* SRC_AST_STRING_CONSTANT_HPP_ */
