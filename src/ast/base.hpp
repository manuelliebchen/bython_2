/*
 * ASTBase.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_BASE_HPP_
#define SRC_AST_BASE_HPP_

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include <peglib.h>

namespace by::ast {

class ASTBlockExpression;

/**
 * error for all ast related errors, prints position in code
 */
struct ast_error : public std::runtime_error {
  ast_error(const std::shared_ptr<peg::Ast> ast, std::string what_str)
      : runtime_error(ast->path + ":" + std::to_string(ast->line) + ":" +
                      std::to_string(ast->column) + ": " + what_str) {}
};

/**
 * Set for Strings
 */
using identifier_set = std::unordered_set<std::string>;

/**
 * Bast class for all ast nodes in ast
 */
class ASTBase {
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

  ASTBase() = delete;

public:
  /**
   * Baic Constructor
   */
  ASTBase(const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
      : ast(ast), parent(parent){};
  virtual ~ASTBase(){};

  /**
   * Saves dependencies to functions and types
   */
  virtual void get_dependencies(identifier_set &functions,
                                identifier_set &types) const = 0;

  /**
   * returns peg::Ast pointer of this node
   */
  const std::shared_ptr<peg::Ast> &get_ast() const { return ast; };

  friend std::ostream &operator<<(std::ostream &, const ASTBase &);
};

/**
 * Print name of the node.
 */
inline std::ostream &operator<<(std::ostream &os, const ASTBase &base) {
  os << base.ast->original_name;
  return os;
}
} /* namespace by::ast */

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

#endif /* SRC_AST_BASE_HPP_ */
