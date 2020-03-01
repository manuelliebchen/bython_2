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

namespace by {
namespace bc {
struct BuildContext;
} // namespace bc
} // namespace by

namespace by::ast {

class ASTFunction;
class ASTExtern;

class ASTRoot : public ASTBase {
public:
  ASTRoot(const std::shared_ptr<peg::Ast> &);

  void compile(std::ostream &);

private:
  void reorder_functions();

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

  std::vector<std::shared_ptr<by::ast::ASTFunction>> functions;
  std::vector<std::shared_ptr<by::ast::ASTExtern>> externs;

  friend std::ostream &operator<<(std::ostream &, const ASTRoot &);
};

std::ostream &operator<<(std::ostream &, const ASTRoot &);

} /* namespace by::ast */

#endif /* SRC_ASTROOT_HPP_ */
