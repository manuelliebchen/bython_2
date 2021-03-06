/*
 * ASTExpression.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTEXPRESSION_HPP_
#define SRC_AST_ASTEXPRESSION_HPP_

#include <llvm/IR/Value.h>
#include <peglib.h>
#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_set>

#include "bc/build_context.h"
#include "type/type_name.h"
#include "ast/ast_error.h"

namespace by {
namespace bc {
class BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {

class ASTBlockExpression;

/**
 * Set for Strings
 */
using identifier_set = std::unordered_set<std::string>;

/**
 * AST node for all ast nodes that have a type.
 * May be merget with ASTBase.
 */
class ASTExpression {
protected:
  /**
   * Pointer to peg::Ast object of this AST node
   */
  const std::shared_ptr<peg::Ast> ast;

  /**
   * Pointer to Blockexpression for looking up variables or registration of
   * varibles
   */
  ASTBlockExpression *parent;

  /**
   * type of the expression
   */
  type::TypeName_ptr type;

  ASTExpression() = delete;
  /**
   * ctor to copy pointers
   */
  ASTExpression(std::shared_ptr<peg::Ast>, ASTBlockExpression *);

  /**
   * Creates expression from given input, this will not be an ASTExpression.
   */
  static std::shared_ptr<by::ast::ASTExpression>
  create_expression(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

public:
  virtual ~ASTExpression(){};
  /**
   * Function to determine type of this expression and all child expressions
   */
  virtual type::TypeName_ptr determine_type(std::unique_ptr<bc::BuildContext> &) = 0;

  /**
   * Builds ir code and returns llvm::Value* for further use in parent
   * expression.
   */
  virtual llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const = 0;

  /**
   * returns peg::Ast pointer of this node
   */
  const std::shared_ptr<peg::Ast> &get_ast() const { return ast; };

  friend std::ostream &operator<<(std::ostream &,
                                  const by::ast::ASTExpression &);
};

std::ostream &operator<<(std::ostream &, const by::ast::ASTExpression &);
} // namespace by::ast

namespace std {

/**
 * returns string of ast or throws an ast_error
 */
inline auto to_string(const std::shared_ptr<peg::Ast> &ast) -> std::string {
  if (ast->original_name == "Identifier" && ast->is_token) {
    return ast->token;
  }
  throw by::ast::ast_error(ast, "Expected Identifier");
};
} // namespace std

/**
 * Helpfull marker for debuging purpose
 */
#define CHECK                                                                  \
  {                                                                            \
    std::cerr << "Success: " << __FILE__ << ", line "                          \
              << std::to_string(__LINE__) << std::endl;                        \
  }

#endif /* SRC_AST_ASTEXPRESSION_HPP_ */
