/*
 * ASTBlockExpression.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTBLOCKEXPRESSION_HPP_
#define SRC_AST_ASTBLOCKEXPRESSION_HPP_

#include <llvm/IR/Value.h>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../type/type_name.h"
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

class ASTVariableDeclaration;

class ASTBlockExpression : public ASTExpression {
protected:
  std::vector<std::shared_ptr<ASTExpression>> expressions;
  std::shared_ptr<ASTExpression> end_expression;

  by::type::variable_map known_variables;

public:
  ASTBlockExpression(const std::shared_ptr<peg::Ast> &ast,
                     ASTBlockExpression *);
  ASTBlockExpression(
      const std::vector<std::shared_ptr<by::ast::ASTVariableDeclaration>> &,
      const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *);

  by::type::TypeName_ptr determine_type(std::unique_ptr<bc::BuildContext> &);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

  by::type::TypeName_ptr find_variable_type(const std::string &) const;

  /**
   * @return false if variable already exists.
   */
  bool register_variable(const std::string &, const by::type::TypeName_ptr &);

  friend std::ostream &operator<<(std::ostream &, const ASTBlockExpression &);
};

inline std::ostream &operator<<(std::ostream &os,
                                const ASTBlockExpression &block) {
  os << "{\n";
  for (auto &exp : block.expressions) {
    os << *exp << ";" << std::endl;
  }
  os << "}";
  return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_ASTBLOCKEXPRESSION_HPP_ */
