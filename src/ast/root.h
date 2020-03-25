/*
 * ProgrammAst.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_ASTROOT_HPP_
#define SRC_ASTROOT_HPP_

#include <peglib.h>
#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "../type/type_name.h"
#include "extern.h"
#include "function.h"
#include "import.h"

namespace by {
namespace bc {
class BuildContext;
} // namespace bc
} // namespace by

namespace by::ast {

class ASTFunction;
class ASTExtern;
class ASTImport;

/**
 * Top most AST node of tree. Containg all functions and externs
 */
class ASTRoot {
private:
	std::string file;

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
   * Imports declarations
   */
  std::vector<std::shared_ptr<by::ast::ASTImport>> imports;

  std::shared_ptr<std::unordered_set<std::string>> file_list;

public:
  ASTRoot(std::string, const std::shared_ptr<peg::Ast> &);
  ASTRoot(std::string, const std::shared_ptr<peg::Ast> &, std::shared_ptr<std::unordered_set<std::string>>);

  void insert_functions(
      std::unique_ptr<by::bc::BuildContext> &) const;

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
