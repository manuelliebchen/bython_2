/*
 * ASTBase.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTBASE_HPP_
#define SRC_AST_ASTBASE_HPP_

#include <memory>
#include <string>
#include <unordered_set>

#include "peglib.h"
#include "utillib.hpp"

namespace by::ast {

class ASTBlockExpression;

struct bad_ast_exeption : std::exception {
	const std::shared_ptr<peg::Ast> ast;
	const std::string name;
	bad_ast_exeption(const std::shared_ptr<peg::Ast> ast, const char* name)
		: ast(ast), name(std::string(name))
	{
	}
	[[nodiscard]] const char* what() const noexcept override
	{
		return (ast->path + std::string(":") + std::to_string(ast->line) +
				std::string(":") + std::to_string(ast->column) +
				std::string(":") + name)
			.c_str();
	}
};

using identifier_set = std::unordered_set<std::string>;

class ASTBase {
  protected:
	const std::shared_ptr<peg::Ast> ast;

	ASTBlockExpression* parent;

	ASTBase() = delete;

  public:
	ASTBase(const std::shared_ptr<peg::Ast>& ast, ASTBlockExpression* parent)
		: ast(ast), parent(parent){};
	virtual ~ASTBase(){};

	virtual void get_dependencies(identifier_set& functions,
								  identifier_set& types) const = 0;

	const std::shared_ptr<peg::Ast>& get_ast() const
	{
		return ast;
	};

	friend std::ostream& operator<<(std::ostream&, const ASTBase&);
};

inline std::ostream& operator<<(std::ostream& os, const ASTBase& base)
{
	os << base.ast->original_name;
	return os;
}

} /* namespace by::ast */

#endif /* SRC_AST_ASTBASE_HPP_ */
