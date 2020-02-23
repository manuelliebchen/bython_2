/*
 * ASTBooleanConstant.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTCONSTANT_ASTBOOLEANCONSTANT_HPP_
#define SRC_AST_ASTCONSTANT_ASTBOOLEANCONSTANT_HPP_

#include "../../ast/ASTConstant/ASTConstant.hpp"

namespace by::ast {

class ASTBooleanConstant : public ASTConstant {
  public:
	ASTBooleanConstant(const std::shared_ptr<peg::Ast>&, ASTBlockExpression*);

	llvm::Value* build_ir(std::unique_ptr<bc::BuildContext>&) const;

	void get_dependencies(std::unordered_set<std::string>& functions,
						  std::unordered_set<std::string>& types) const;

  private:
	bool value;

	friend std::ostream& operator<<(std::ostream&, const ASTBooleanConstant&);
};

inline std::ostream& operator<<(std::ostream& os, const ASTBooleanConstant& boo)
{
	if (boo.value) {
		os << "true";
	}
	else {
		os << "false";
	}
	return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_ASTCONSTANT_ASTBOOLEANCONSTANT_HPP_ */
