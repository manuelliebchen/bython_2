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

#include "base.hpp"
#include "extern.hpp"
#include "function.hpp"
#include "type/type_name.hpp"

namespace by::ast {

class ASTFunction;
class ASTExtern;

class ASTRoot : public ASTBase {
public:
  ASTRoot(const std::shared_ptr<peg::Ast> &);

  void reorder_functions();

  void determine_type(type::variable_map &);

  void build_ir(std::unique_ptr<by::bc::BuildContext> &) const;

  const std::vector<std::shared_ptr<by::ast::ASTFunction>> &get_functions();
  const std::vector<std::shared_ptr<by::ast::ASTExtern>> &get_externs();

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
