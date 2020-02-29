/*
 * ProgrammAst.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_ASTROOT_HPP_
#define SRC_ASTROOT_HPP_

#include <memory>
#include <peglib.h>
#include <vector>

#include "base.hpp"
#include "extern.hpp"
#include "function.hpp"

namespace by::ast {

class ASTFunction;
class ASTExtern;

class ASTRoot : public ASTBase {
public:
  ASTRoot(const std::shared_ptr<peg::Ast> &);

  void reorder_functions(const std::vector<std::string> &order);

  void determine_type(type::variable_map &);

  const std::vector<std::shared_ptr<by::ast::ASTFunction>> &get_functions();

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

private:
  std::vector<std::shared_ptr<by::ast::ASTFunction>> functions;
  std::vector<std::shared_ptr<by::ast::ASTExtern>> externs;

  friend std::ostream &operator<<(std::ostream &, const ASTRoot &);
};

std::ostream &operator<<(std::ostream &, const ASTRoot &);

} /* namespace by::ast */

#endif /* SRC_ASTROOT_HPP_ */
