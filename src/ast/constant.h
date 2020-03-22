/*
 * ASTConstant.hpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTCONSTANT_HPP_
#define SRC_AST_ASTCONSTANT_HPP_
#include <iosfwd>             // for ostream
#include <memory>             // for shared_ptr
#include <string>             // for string
#include <unordered_set>      // for unordered_set

#include "peglib.h"           // for Ast
#include "../type/type_name.h" // for TypeName_ptr, variable_map
#include "expression.h" // for ASTExpression

namespace by {
namespace bc {
class BuildContext;
}  // namespace bc
}  // namespace by

namespace by::ast {
class ASTBlockExpression;
}

namespace by::ast {

class ASTConstant : public ASTExpression {
protected:
  std::string const_string;
  ASTConstant(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *,
              std::string);

public:
  by::type::TypeName_ptr determine_type(std::unique_ptr<bc::BuildContext> &);

  friend std::ostream &operator<<(std::ostream &, const ASTConstant &);
};

std::ostream &operator<<(std::ostream &, const ASTConstant &);

} /* namespace by::ast */

#endif /* SRC_AST_ASTCONSTANT_HPP_ */
