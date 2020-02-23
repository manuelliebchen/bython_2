/*
 * VariableDeclaration.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#ifndef SRC_AST_VARIABLE_DECLARATION_HPP_
#define SRC_AST_VARIABLE_DECLARATION_HPP_

#include <memory>

#include "../type/TypeName.hpp"
#include "ASTExpression/ASTBlockExpression.hpp"
#include "base.hpp"
#include "peglib.h"

namespace by::ast {

class ASTVariableDeclaration : public ASTBase {
  protected:
	std::string name;
	by::type::TypeName type;

  public:
	ASTVariableDeclaration(const std::shared_ptr<peg::Ast>&,
						   ASTBlockExpression*);

	void get_dependencies(std::unordered_set<std::string>& functions,
						  std::unordered_set<std::string>& types) const;

	const std::string& get_name() const;
	by::type::TypeName get_type() const;
	friend std::ostream& operator<<(std::ostream&,
									const ASTVariableDeclaration&);
};

inline std::ostream& operator<<(std::ostream& os,
								const ASTVariableDeclaration& var)
{
	os << var.name << " : " << var.type;
	return os;
}

} // namespace by::ast

#endif /* SRC_AST_VARIABLE_DECLARATION_HPP_ */
