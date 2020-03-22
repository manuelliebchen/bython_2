/*
 * ASTVariableExpression.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTVARIABLEEXPRESSION_HPP_
#define SRC_AST_ASTVARIABLEEXPRESSION_HPP_

#include <llvm/IR/Value.h>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>

#include "../type/type_name.h"
#include "expression.h"
#include "peglib.h"

namespace by {
namespace ast {
class ASTBlockExpression;
} // namespace ast
namespace bc {
class BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {

class ASTVariableExpression : public ASTExpression {
private:
  std::string name;
  std::shared_ptr<ASTVariableExpression> next;
  friend std::ostream &operator<<(std::ostream &,
                                  const ASTVariableExpression &);

public:
  ASTVariableExpression(const std::shared_ptr<peg::Ast> &,
                        ASTBlockExpression *);

  by::type::TypeName_ptr determine_type(std::unique_ptr<bc::BuildContext> &);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

  std::string get_name() const {
    return name;
  }
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
