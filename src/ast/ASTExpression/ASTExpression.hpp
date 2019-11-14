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

#include "../../ast/ASTBase.hpp"
#include "../../bc/BuildContext.hpp"
#include "../../type/TypeName.hpp"
#include "peglib.h"
#include "utillib.hpp"

namespace by::ast {

class ASTExpression : public ASTBase {
  public:
	ASTExpression(const std::shared_ptr<peg::Ast>&);

	virtual llvm::Value* build_ir(std::unique_ptr<bc::BuildContext>&) const = 0;

	friend std::ostream& operator<<(std::ostream&,
									const by::ast::ASTExpression&);
};

std::ostream& operator<<(std::ostream&, const by::ast::ASTExpression&);

std::shared_ptr<by::ast::ASTExpression>
create_expression(const std::shared_ptr<peg::Ast>&);
} // namespace by::ast

#endif /* SRC_AST_ASTEXPRESSION_HPP_ */
