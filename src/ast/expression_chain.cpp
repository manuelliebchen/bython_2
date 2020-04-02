#include "expression_chain.h"

#include <bc/function_priority.h>
#include <cstddef>
#include <ext/alloc_traits.h>
#include <memory>
#include <peglib.h>
#include <utility>
#include <vector>

#include "arithmetic_expression.h"
#include "ast/ast_error.h"
#include "ast/expression.h"
#include "bc/build_context.h"
#include "bc/function_build.h"
#include "bc/function_manager.h"

namespace by::ast {
class ASTBlockExpression;

ExpressionChain::ExpressionChain(
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent,
    std::vector<std::shared_ptr<ASTExpression>> expressions,
    std::vector<std::string> operators)
    : ASTExpression(ast, parent), expressions(std::move(expressions)),
      operators(std::move(operators)) {}

auto ExpressionChain::determine_type(std::unique_ptr<bc::BuildContext> &bc)
    -> by::type::TypeName_ptr {

  if (*type == *type::TypeName::None) {
    for (auto &exp : expressions) {
      exp->determine_type(bc);
    }

    for (auto &operator_class : bc->functions.get_operators()) {
      for (size_t j = 0; j < operators.size(); ++j) {
        std::vector<type::TypeName_ptr> param{
            expressions[j]->determine_type(bc),
            expressions[j + 1]->determine_type(bc)};
        auto res = operator_class.equal_range(operators[j]);
        for (auto i = res.first; i != res.second; ++i) {
          if (i->second.get_type()->param_equal(param)) {
            expressions[j] = std::make_shared<ASTArithmeticExpression>(
                ast, parent, expressions[j], operators[j], expressions[j + 1]);
            expressions.erase(expressions.begin() + j + 1);
            operators.erase(operators.begin() + j);
            j--;
            break;
          }
        }
      }
    }
    if (!operators.empty()) {
      throw ast_error(ast, "More then one Expression on top, Expressions: " +
                               operators[0]);
    }
    type = expressions.back()->determine_type(bc);
  }
  return type;
}

auto ExpressionChain::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  return expressions.back()->build_ir(bc);
}

auto operator<<(std::ostream &os, const ExpressionChain &exp)
    -> std::ostream & {
  os << *exp.expressions.front();
  for (size_t i = 0; i < exp.operators.size(); ++i) {
    os << " " << exp.operators[i] << " " << *exp.expressions[i + 1];
  }
  return os;
};

} /* namespace by::ast */
// namespace std
