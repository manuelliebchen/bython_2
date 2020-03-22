#include "expression_chain.h"

#include <ext/alloc_traits.h>
#include <memory>
#include <utility>
#include <vector>

#include "arithmetic_expression.h"
#include "ast/expression.h"
#include "peglib.h"

namespace by {
namespace ast {
class ASTBlockExpression;
} // namespace ast
namespace bc {
class BuildContext;
} // namespace bc
} // namespace by

namespace by::ast {

ExpressionChain::ExpressionChain(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent,
    std::vector<std::shared_ptr<ASTExpression>> expressions,
    std::vector<std::string> operators)
    : ASTExpression(ast, parent), expressions(std::move(expressions)),
      operators(std::move(operators)) {}

auto ExpressionChain::determine_type(std::unique_ptr<bc::BuildContext> &bc)
    -> by::type::TypeName_ptr {

  int min_prio = 0;
  int max_prio = 0;
  for (auto &fun : *bc) {
    int prio = fun.get_priority();
    if (prio < min_prio) {
      min_prio = prio;
    }
    if (prio > max_prio) {
      max_prio = prio;
    }
  }

  for (int i = min_prio; i <= max_prio; ++i) {
    for (auto &fun : *bc) {
      if (fun.get_priority() == i) {
        for (size_t j = 0; j < operators.size(); ++j) {
          if (fun.get_name() == operators[j]) {
            expressions[j] = std::make_shared<ASTArithmeticExpression>(
                ast, parent, expressions[j], operators[j], expressions[j + 1]);
            expressions.erase(expressions.begin() + j + 1);
            operators.erase(operators.begin() + j);
          }
        }
      }
    }
  }
  if (expressions.size() > 1) {
    throw ast_error(ast, "More then one Expression on top");
  }
  return expressions.back()->determine_type(bc);
}

auto ExpressionChain::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  return expressions.back()->build_ir(bc);
}
} /* namespace by::ast */
// namespace std
