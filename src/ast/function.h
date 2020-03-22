/*
 * ASTFunction.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_ASTFUNCTION_HPP_
#define SRC_ASTFUNCTION_HPP_

#include <iosfwd>
#include <llvm/IR/Value.h>
#include <memory>
#include <peglib.h>
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
namespace ast {
class ASTBlockExpression;
class ASTVariableDeclaration;
} // namespace ast
namespace bc {
class BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {

/**
 * AST node for function declarations
 */

class ASTFunction : public ASTExpression {
protected:
  /**
   * name of the functino
   */
  std::string name;

  type::FunctionType_ptr function_type;

  /**
   * parameters of the function
   */
  std::vector<std::shared_ptr<by::ast::ASTVariableDeclaration>> parameters;

  /**
   * content of the function
   */
  std::shared_ptr<by::ast::ASTBlockExpression> blockexpression;

public:
  ASTFunction(const std::shared_ptr<peg::Ast> &);

  type::TypeName_ptr determine_type(std::unique_ptr<bc::BuildContext> &);

  std::string get_name() const;

  void insertFunction(std::unique_ptr<by::bc::BuildContext> &) const;
  llvm::Value *build_ir(std::unique_ptr<by::bc::BuildContext> &) const;

  friend std::ostream &operator<<(std::ostream &, const ASTFunction &);
};

std::ostream &operator<<(std::ostream &, const ASTFunction &);

} // namespace by::ast

#endif /* SRC_ASTFUNCTION_HPP_ */
