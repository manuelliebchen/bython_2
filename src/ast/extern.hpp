/*
 * ASTFunction.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_ASTEXTERN_HPP_
#define SRC_ASTEXTERN_HPP_

#include <bc/build_context.hpp>
#include <iosfwd>
#include <llvm/IR/Value.h>
#include <memory>
#include <peglib.h>
#include <string>
#include <type/function_type.hpp>
#include <type/type_name.hpp>
#include <unordered_set>
#include <vector>

#include "ast/expression/expression.hpp"
#include "base.hpp"
#include "expression/block_expression.hpp"
#include "variable_declaration.hpp"

namespace by {
namespace bc {
struct BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {

/**
 * AST node for extern declarations of functions
 */
class ASTExtern : public ASTExpression {
protected:
  /**
   * Name of the function
   */
  std::string name;

public:
  ASTExtern(const std::shared_ptr<peg::Ast> &);

  type::TypeName_ptr determine_type(type::variable_map &);

  /**
   * returns name of the function
   */
  std::string get_name() const;
  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

  llvm::Value *build_ir(std::unique_ptr<by::bc::BuildContext> &) const;

  friend std::ostream &operator<<(std::ostream &, const ASTExtern &);
};

std::ostream &operator<<(std::ostream &, const ASTExtern &);

} // namespace by::ast

#endif /* SRC_ASTEXTERN_HPP_ */
