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

#include "block_expression.hpp"
#include "expression.hpp"
#include "peglib.h"
#include "type/type_name.hpp"

namespace by {
namespace bc {
struct BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {

class ASTIfExpression : public ASTExpression {
public:
  ASTIfExpression(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

  by::type::TypeName_ptr determine_type(type::variable_map &);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

private:
  std::shared_ptr<ASTExpression> condition;
  std::shared_ptr<ASTBlockExpression> block;
  std::shared_ptr<ASTExpression> alternativ;

  friend std::ostream &operator<<(std::ostream &, const ASTIfExpression &);
};

inline std::ostream &operator<<(std::ostream &os,
                                const ASTIfExpression &ifexp) {
  os << "if " << *ifexp.condition << " " << *ifexp.block;
  if (ifexp.alternativ != nullptr) {
    os << *ifexp.alternativ;
  }
  return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_ASTIFEXPRESSION_HPP_ */
