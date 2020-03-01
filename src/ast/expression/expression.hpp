/*
 * ASTExpression.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTEXPRESSION_HPP_
#define SRC_AST_ASTEXPRESSION_HPP_

#include <bc/build_context.hpp>
#include <iosfwd>
#include <llvm/IR/Value.h>
#include <memory>
#include <peglib.h>
#include <type/type_name.hpp>

#include "../base.hpp"

namespace by {
namespace bc {
struct BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {

class ASTBlockExpression;

class ASTExpression : public ASTBase {
protected:
  by::type::TypeName_ptr type;

public:
  ASTExpression(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

  virtual by::type::TypeName_ptr determine_type(type::variable_map &) = 0;

  virtual llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const = 0;

  virtual by::type::TypeName_ptr get_type() const;

  friend std::ostream &operator<<(std::ostream &,
                                  const by::ast::ASTExpression &);
};

std::ostream &operator<<(std::ostream &, const by::ast::ASTExpression &);

std::shared_ptr<by::ast::ASTExpression>
create_expression(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);
} // namespace by::ast

#endif /* SRC_AST_ASTEXPRESSION_HPP_ */
