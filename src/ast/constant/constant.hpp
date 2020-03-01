/*
 * ASTConstant.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTCONSTANT_HPP_
#define SRC_AST_ASTCONSTANT_HPP_
#include "../expression/expression.hpp" // for ASTExpression
#include "peglib.h"                     // for Ast
#include "type/type_name.hpp"           // for TypeName_ptr, variable_map
#include <iosfwd>                       // for ostream
#include <memory>                       // for shared_ptr
#include <string>                       // for string
#include <unordered_set>                // for unordered_set
namespace by::ast {
class ASTBlockExpression;
}

namespace by::ast {

class ASTConstant : public ASTExpression {
private:
  std::string const_string;

public:
  ASTConstant(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *,
              std::string);

  by::type::TypeName_ptr determine_type(type::variable_map &);

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

  friend std::ostream &operator<<(std::ostream &, const ASTConstant &);
};

std::ostream &operator<<(std::ostream &, const ASTConstant &);

} /* namespace by::ast */

#endif /* SRC_AST_ASTCONSTANT_HPP_ */
