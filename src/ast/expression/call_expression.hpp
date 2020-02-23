/*
 * ASTCallExpression.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTCALLEXPRESSION_HPP_
#define SRC_AST_ASTCALLEXPRESSION_HPP_

#include "expression.hpp"

namespace by::ast {

class ASTCallExpression : public ASTExpression {
  public:
	ASTCallExpression(const std::shared_ptr<peg::Ast>&, ASTBlockExpression*);

	llvm::Value* build_ir(std::unique_ptr<bc::BuildContext>&) const;

	void get_dependencies(std::unordered_set<std::string>& functions,
						  std::unordered_set<std::string>& types) const;

  private:
	std::string name;
	std::vector<std::shared_ptr<ASTExpression>> arguments;
	friend std::ostream& operator<<(std::ostream&, const ASTCallExpression&);
};

inline std::ostream& operator<<(std::ostream& os, const ASTCallExpression& call)
{
	os << call.name << "( ";
	for (auto& arg : call.arguments) {
		os << *arg << ", ";
	}
	os << ")";
	return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_ASTCALLEXPRESSION_HPP_ */
