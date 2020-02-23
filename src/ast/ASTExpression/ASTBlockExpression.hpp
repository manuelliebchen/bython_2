/*
 * ASTBlockExpression.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTBLOCKEXPRESSION_HPP_
#define SRC_AST_ASTBLOCKEXPRESSION_HPP_

#include <memory>
#include <vector>

#include "ASTExpression.hpp"

namespace by::ast {

class ASTVariableDeclaration;

class ASTBlockExpression : public ASTExpression {
  protected:
	std::vector<std::shared_ptr<ASTExpression>> expressions;

	by::type::variable_map known_variables;

  public:
	ASTBlockExpression(const std::shared_ptr<peg::Ast>& ast,
					   ASTBlockExpression*);
	ASTBlockExpression(
		const std::vector<std::shared_ptr<by::ast::ASTVariableDeclaration>>&,
		const std::shared_ptr<peg::Ast>& ast,
		ASTBlockExpression*);

	llvm::Value* build_ir(std::unique_ptr<bc::BuildContext>&) const;

	void get_dependencies(std::unordered_set<std::string>& functions,
						  std::unordered_set<std::string>& types) const;

	by::type::TypeName find_variable_type(const std::string&) const;

	/**
	 * @return false if variable already exists.
	 */
	bool register_variable(const std::string&, const by::type::TypeName&);

	friend std::ostream& operator<<(std::ostream&, const ASTBlockExpression&);
};

inline std::ostream& operator<<(std::ostream& os,
								const ASTBlockExpression& block)
{
	os << "{\n";
	for (auto& exp : block.expressions) {
		os << *exp << ";" << std::endl;
	}
	os << "}";
	return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_ASTBLOCKEXPRESSION_HPP_ */
