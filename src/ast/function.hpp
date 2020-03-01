/*
 * ASTFunction.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_ASTFUNCTION_HPP_
#define SRC_ASTFUNCTION_HPP_

#include <bc/build_context.hpp>
#include <iosfwd>
#include <memory>
#include <peglib.h>
#include <string>
#include <type/function_type.hpp>
#include <type/type_name.hpp>
#include <unordered_set>
#include <util/util.hpp>
#include <vector>

#include "ast/expression/expression.hpp"
#include "base.hpp"
#include "expression/block_expression.hpp"
#include "variable_declaration.hpp"

namespace by {
namespace ast {
class ASTBlockExpression;
class ASTVariableDeclaration;
} // namespace ast
namespace bc {
struct BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {

class ASTFunction : public ASTExpression {
protected:
  std::string name;
  std::vector<std::shared_ptr<by::ast::ASTVariableDeclaration>> parameters;
  std::shared_ptr<by::ast::ASTBlockExpression> blockexpression;

public:
  ASTFunction(const std::shared_ptr<peg::Ast> &);

  type::TypeName_ptr determine_type(type::variable_map &);

  std::string get_name() const;
  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

  llvm::Value *build_ir(std::unique_ptr<by::bc::BuildContext> &) const;

  friend std::ostream &operator<<(std::ostream &, const ASTFunction &);
};

std::ostream &operator<<(std::ostream &, const ASTFunction &);

} // namespace by::ast

#endif /* SRC_ASTFUNCTION_HPP_ */
