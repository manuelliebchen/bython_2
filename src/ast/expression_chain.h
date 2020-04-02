#ifndef __EXPRESSION_CHAIN_H
#define __EXPRESSION_CHAIN_H

#include <llvm/IR/Value.h>
#include <vector>
#include <memory>
#include <string>
#include <unordered_set>

#include "block_expression.h"
#include "expression.h"
#include "peglib.h"
#include "type/type_name.h"

namespace by {
namespace ast {
class ASTBlockExpression;
}  // namespace ast
namespace bc {
class BuildContext;
}  // namespace bc
}  // namespace by

namespace by::ast{

class ExpressionChain : public ASTExpression {
  std::vector<std::shared_ptr<ASTExpression>> expressions;
  std::vector<std::string> operators;

public:
  ExpressionChain(const std::shared_ptr<peg::Ast> &,
                          ASTBlockExpression *, std::vector<std::shared_ptr<ASTExpression>>, std::vector<std::string>);

  by::type::TypeName_ptr determine_type(std::unique_ptr<bc::BuildContext> &);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;
  friend std::ostream &operator<<(std::ostream &, const ExpressionChain &);
};

std::ostream &operator<<(std::ostream &, const ExpressionChain &);

}

#endif //__EXPRESSION_CHAIN_H
