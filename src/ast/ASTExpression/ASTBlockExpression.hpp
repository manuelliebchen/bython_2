/*
 * ASTBlockExpression.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTBLOCKEXPRESSION_HPP_
#define SRC_AST_ASTBLOCKEXPRESSION_HPP_

#include "../../ast/ASTExpression/ASTExpression.hpp"

namespace by::ast {

class ASTBlockExpression : public ASTExpression {
  public:
	ASTBlockExpression(const std::shared_ptr<peg::Ast>& ast);

	llvm::Value* build_ir(std::unique_ptr<bc::BuildContext>&) const;

	void get_dependencies(std::unordered_set<std::string>& functions,
						  std::unordered_set<std::string>& types) const;

  private:
	std::vector<std::shared_ptr<ASTExpression>> expressions;

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
