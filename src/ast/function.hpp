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

#include "../bc/build_context.hpp"
#include "../type/function_type.hpp"
#include "../type/type_name.hpp"
#include "base.hpp"
#include "expression/block_expression.hpp"
#include "peglib.h"
#include "variable_declaration.hpp"

namespace by::ast {

class ASTFunction : public ASTBase {
protected:
  std::string name;
  std::vector<std::shared_ptr<by::ast::ASTVariableDeclaration>> parameters;
  std::shared_ptr<by::type::TypeName> returntype;
  std::shared_ptr<by::ast::ASTBlockExpression> blockexpression;

public:
  ASTFunction(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

  void determine_type(const type::function_map &);

  std::string get_name() const;
  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

  void build_ir(std::unique_ptr<by::bc::BuildContext> &) const;

  type::FunctionType get_function_type() const;

  friend std::ostream &operator<<(std::ostream &, const ASTFunction &);
};

std::ostream &operator<<(std::ostream &, const ASTFunction &);

} // namespace by::ast

#endif /* SRC_ASTFUNCTION_HPP_ */
