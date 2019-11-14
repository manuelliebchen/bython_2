/*
 * VariableDeclaration.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#ifndef SRC_AST_ASTVARIABLEDECLARATION_HPP_
#define SRC_AST_ASTVARIABLEDECLARATION_HPP_

#include <memory>

#include "../ast/ASTBase.hpp"
#include "../type/TypeName.hpp"
#include "peglib.h"

namespace by::ast {

class ASTVariableDeclaration : public ASTBase {
  public:
	ASTVariableDeclaration(const std::shared_ptr<peg::Ast>&);

	void get_dependencies(std::unordered_set<std::string>& functions,
						  std::unordered_set<std::string>& types) const;

	const std::string& get_name() const;
	const std::shared_ptr<by::type::TypeName>& get_type() const;

  private:
	std::string name;
	std::shared_ptr<by::type::TypeName> type;
	friend std::ostream& operator<<(std::ostream&,
									const ASTVariableDeclaration&);
};

inline std::ostream& operator<<(std::ostream& os,
								const ASTVariableDeclaration& var)
{
	os << var.name << " : " << *var.type;
	return os;
}

} // namespace by::ast

#endif /* SRC_AST_ASTVARIABLEDECLARATION_HPP_ */
