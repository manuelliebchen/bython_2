/*
 * ASTFunction.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_ASTEXTERN_HPP_
#define SRC_ASTEXTERN_HPP_

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

class ASTExtern : public ASTExpression {
protected:
  std::string name;

public:
  ASTExtern(const std::shared_ptr<peg::Ast> &);

  type::TypeName_ptr determine_type(type::variable_map &);

  std::string get_name() const;
  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

  llvm::Value *build_ir(std::unique_ptr<by::bc::BuildContext> &) const;

  friend std::ostream &operator<<(std::ostream &, const ASTExtern &);
};

std::ostream &operator<<(std::ostream &, const ASTExtern &);

} // namespace by::ast

#endif /* SRC_ASTEXTERN_HPP_ */
