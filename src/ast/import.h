/*
 * ASTImport.hpp
 *
 *  Created on: Mar 24, 2020
 *      Author: Manuel Liebchen
 */

#ifndef SRC_ASTIMPORT_HPP_
#define SRC_ASTIMPORT_HPP_

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
#include "variable_declaration.h"
#include "root.h"
#include "ast/extern.h"

namespace by {
namespace bc {
class BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {
class ASTRoot;

/**
 * AST node for extern declarations of functions
 */
class ASTImport {
protected:
  /**
   * Name of the function
   */
  std::string file;
  std::shared_ptr<by::ast::ASTRoot> root;

public:
  ASTImport(const std::shared_ptr<peg::Ast> &,const std::shared_ptr<std::unordered_set<std::string>>&);

  void insert_functions(
      std::unique_ptr<by::bc::BuildContext> &) const;

  /**
   * returns name of the function
   */
  std::string get_file_name() const;

  friend std::ostream &operator<<(std::ostream &, const ASTImport &);
};

std::ostream &operator<<(std::ostream &, const ASTExtern &);

} // namespace by::ast

#endif /* SRC_ASTIMPORT_HPP_ */
