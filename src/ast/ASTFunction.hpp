/*
 * ASTFunction.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_ASTFUNCTION_HPP_
#define SRC_ASTFUNCTION_HPP_

#include <memory>
#include <string>
#include <utillib.hpp>
#include <vector>

#include "../ast/ASTBase.hpp"
#include "../ast/ASTExpression/ASTBlockExpression.hpp"
#include "../ast/ASTVariableDeclaration.hpp"
#include "../bc/BuildContext.hpp"
#include "../type/FunctionType.hpp"
#include "../type/TypeName.hpp"
#include "peglib.h"

namespace by::ast {

class ASTFunction : public ASTBase {
  protected:
	std::string name;
	std::vector<std::shared_ptr<by::ast::ASTVariableDeclaration>> parameters;
	std::shared_ptr<by::type::TypeName> returntype;
	std::shared_ptr<by::ast::ASTBlockExpression> blockexpression;

  public:
	ASTFunction(const std::shared_ptr<peg::Ast>&, ASTBlockExpression*);

	std::string get_name() const;
	void get_dependencies(std::unordered_set<std::string>& functions,
						  std::unordered_set<std::string>& types) const;

	void build_ir(std::unique_ptr<by::bc::BuildContext>&) const;

	type::FunctionType get_function_type() const;

	friend std::ostream& operator<<(std::ostream&, const ASTFunction&);
};

std::ostream& operator<<(std::ostream&, const ASTFunction&);

} // namespace by::ast

#endif /* SRC_ASTFUNCTION_HPP_ */
