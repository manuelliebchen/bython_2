/*
 * ProgrammAst.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_ASTROOT_HPP_
#define SRC_ASTROOT_HPP_

#include <iosfwd>
#include <memory>
#include <peglib.h>
#include <string>
#include <unordered_set>
#include <vector>

#include "../type/type_name.h"
#include "extern.h"
#include "function.h"

namespace by {
namespace bc {
struct BuildContext;
} // namespace bc
} // namespace by

namespace by::ast {

class ASTFunction;
class ASTExtern;

/**
 * Top most AST node of tree. Containg all functions and externs
 */
class ASTRoot {
private:
  /**
   * Pointer to peg::Ast object of this AST node
   */
  const std::shared_ptr<peg::Ast> ast;
  /**
   * Functions of the programm
   */
  std::vector<std::shared_ptr<by::ast::ASTFunction>> functions;
  /**
   * Extern declarations
   */
  std::vector<std::shared_ptr<by::ast::ASTExtern>> externs;
  /**
   * Sorts functions by dependencies
   */
  void reorder_functions();

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

public:
  ASTRoot(const std::shared_ptr<peg::Ast> &);

  /**
   * Function for compiling to ir code.
   * Writes output to @1
   */
  void compile(std::ostream &);

  friend std::ostream &operator<<(std::ostream &, const ASTRoot &);
};

std::ostream &operator<<(std::ostream &, const ASTRoot &);

} /* namespace by::ast */

#endif /* SRC_ASTROOT_HPP_ */
