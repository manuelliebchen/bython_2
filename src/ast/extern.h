/*
 * ASTFunction.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_ASTEXTERN_HPP_
#define SRC_ASTEXTERN_HPP_

#include <llvm/IR/Value.h>
#include <peglib.h>
#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "../bc/build_context.h"
#include "../type/function_type.h"
#include "../type/type_name.h"
#include "block_expression.h"
#include "expression.h"
#include "variable_declaration.h"

namespace by {
namespace bc {
class BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {

/**
 * AST node for extern declarations of functions
 */
class ASTExtern {
protected:
  /**
   * Name of the function
   */
  std::string name;
  type::FunctionType_ptr function_type;

public:
  ASTExtern(const std::shared_ptr<peg::Ast> &);

  void insert_functions(
      std::unique_ptr<by::bc::BuildContext> &) const;

  /**
   * returns name of the function
   */
  std::string get_name() const;
  type::FunctionType_ptr get_function_type() const {return function_type;};

  friend std::ostream &operator<<(std::ostream &, const ASTExtern &);
};

std::ostream &operator<<(std::ostream &, const ASTExtern &);

} // namespace by::ast

#endif /* SRC_ASTEXTERN_HPP_ */
