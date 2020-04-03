/*
 * ASTIfExpression.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTIFEXPRESSION_HPP_
#define SRC_AST_ASTIFEXPRESSION_HPP_

#include <llvm/IR/Value.h>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <unordered_set>
#include <variant>

#include "../type/type_name.h"
#include "block_expression.h"
#include "expression.h"
#include "peglib.h"

namespace by {
namespace bc {
class BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {

class ASTIfExpression : public ASTExpression {
public:
  ASTIfExpression(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

  by::type::TypeName_ptr determine_type(std::unique_ptr<bc::BuildContext> &);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

private:
  std::shared_ptr<ASTExpression> condition;
  std::shared_ptr<ASTBlockExpression> block;
  std::shared_ptr<ASTExpression> alternativ;

  friend std::ostream &operator<<(std::ostream &, const ASTIfExpression &);
};

std::ostream &operator<<(std::ostream &, const ASTIfExpression &);

} /* namespace by::ast */

#endif /* SRC_AST_ASTIFEXPRESSION_HPP_ */
