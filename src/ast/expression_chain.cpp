#include "expression_chain.h"

#include <cstddef>
#include <ext/alloc_traits.h>
#include <memory>
#include <utility>
#include <vector>

#include <peglib.h>

#include "arithmetic_expression.h"
#include "ast/ast_error.h"
#include "ast/expression.h"
#include "bc/build_context.h"
#include "bc/function_build.h"
#include <bc/function_priority.h>

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

  for (int i = bc::FunctionPriority::OPERATOR_DOT;
       i <= bc::FunctionPriority::OPERATOR_LIST; ++i) {
    for (auto &fun : bc->get(static_cast<bc::FunctionPriority>(i))) {
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
