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
#include <vector>

#include <peglib.h>

#include <bc/build_context.hpp>
#include <type/function_type.hpp>
#include <type/type_name.hpp>
#include <util/util.hpp>

#include "base.hpp"
#include "expression/block_expression.hpp"
#include "variable_declaration.hpp"

namespace by::ast {

class ASTFunction : public ASTExpression {
protected:
  std::string name;
  std::vector<std::shared_ptr<by::ast::ASTVariableDeclaration>> parameters;
  std::shared_ptr<by::ast::ASTBlockExpression> blockexpression;

public:
  ASTFunction(const std::shared_ptr<peg::Ast> &);

  type::TypeName_ptr determine_type(type::variable_map &);

  std::string get_name() const;
  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

  llvm::Value *build_ir(std::unique_ptr<by::bc::BuildContext> &) const;

  friend std::ostream &operator<<(std::ostream &, const ASTFunction &);
};

std::ostream &operator<<(std::ostream &, const ASTFunction &);

} // namespace by::ast

#endif /* SRC_ASTFUNCTION_HPP_ */
