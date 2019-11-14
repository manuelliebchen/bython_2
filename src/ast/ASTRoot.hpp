/*
 * ProgrammAst.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_ASTROOT_HPP_
#define SRC_ASTROOT_HPP_

#include <memory>
#include <peglib.h>
#include <vector>

#include "../ast/ASTBase.hpp"
#include "../ast/ASTFunction.hpp"

namespace by::ast {

class ASTRoot : public ASTBase {
  public:
	ASTRoot(const std::shared_ptr<peg::Ast>&);

	const std::vector<std::shared_ptr<by::ast::ASTFunction>>& get_functions();

	void get_dependencies(std::unordered_set<std::string>& functions,
						  std::unordered_set<std::string>& types) const;

  private:
	std::vector<std::shared_ptr<by::ast::ASTFunction>> functions;

	friend std::ostream& operator<<(std::ostream&, const ASTRoot&);
};

std::ostream& operator<<(std::ostream&, const ASTRoot&);

} /* namespace by::ast */

#endif /* SRC_ASTROOT_HPP_ */
