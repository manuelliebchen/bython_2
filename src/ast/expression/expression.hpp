/*
 * ASTExpression.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTEXPRESSION_HPP_
#define SRC_AST_ASTEXPRESSION_HPP_

#include <llvm/IR/Value.h>
#include <memory>

#include "../../bc/build_context.hpp"
#include "../../type/type_name.hpp"
#include "../base.hpp"
#include "peglib.h"
#include "utillib.hpp"

namespace by::ast {

class ASTBlockExpression;

class ASTExpression : public ASTBase {
protected:
  by::type::TypeName type;

public:
  ASTExpression(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

  virtual llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const = 0;

  virtual by::type::TypeName get_type() const;

  friend std::ostream &operator<<(std::ostream &,
                                  const by::ast::ASTExpression &);
};

std::ostream &operator<<(std::ostream &, const by::ast::ASTExpression &);

std::shared_ptr<by::ast::ASTExpression>
create_expression(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);
} // namespace by::ast

#endif /* SRC_AST_ASTEXPRESSION_HPP_ */
