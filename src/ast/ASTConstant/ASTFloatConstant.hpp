/*
 * ASTIntegerConstant.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTFLOATCONSTANT_HPP_
#define SRC_AST_ASTFLOATCONSTANT_HPP_

#include "../../ast/ASTConstant/ASTConstant.hpp"

namespace by::ast {

class ASTFloatConstant : public ASTConstant {
  public:
	ASTFloatConstant(const std::shared_ptr<peg::Ast>&);

	llvm::Value* build_ir(std::unique_ptr<bc::BuildContext>&) const;

	void get_dependencies(std::unordered_set<std::string>& functions,
						  std::unordered_set<std::string>& types) const;

  private:
	float value;

	friend std::ostream& operator<<(std::ostream&, const ASTFloatConstant&);
};

inline std::ostream& operator<<(std::ostream& os, const ASTFloatConstant& in)
{
	os << std::to_string(in.value);
	return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_ASTINTEGERCONSTANT_HPP_ */
