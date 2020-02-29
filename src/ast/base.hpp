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

#include "peglib.h"

namespace by::ast {

class ASTBlockExpression;

struct ast_error : std::runtime_error {
  ast_error(const std::shared_ptr<peg::Ast> ast, std::string what_str)
      : runtime_error(ast->path + ":" + std::to_string(ast->line) + ":" +
                      std::to_string(ast->column) + ": " + what_str) {}
};

using identifier_set = std::unordered_set<std::string>;

class ASTBase {
protected:
  const std::shared_ptr<peg::Ast> ast;

  ASTBlockExpression *parent;

  ASTBase() = delete;

public:
  ASTBase(const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
      : ast(ast), parent(parent){};
  virtual ~ASTBase(){};

  virtual void get_dependencies(identifier_set &functions,
                                identifier_set &types) const = 0;

  const std::shared_ptr<peg::Ast> &get_ast() const { return ast; };

  friend std::ostream &operator<<(std::ostream &, const ASTBase &);
};

inline std::ostream &operator<<(std::ostream &os, const ASTBase &base) {
  os << base.ast->original_name;
  return os;
}

} /* namespace by::ast */

#define CHECK                                                                  \
  {                                                                            \
    std::cerr << "Success: " << __FILE__ << ", line "                          \
              << std::to_string(__LINE__) << std::endl;                        \
  }

#endif /* SRC_AST_BASE_HPP_ */
