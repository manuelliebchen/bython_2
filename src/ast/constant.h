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

template <typename T>
class ASTConstant : public ASTExpression {
protected:
  T value;

public:
  ASTConstant(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

  by::type::TypeName_ptr determine_type(std::unique_ptr<bc::BuildContext> &);
  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

  T get_value() const;

  template <typename U>
  friend std::ostream &operator<<(std::ostream &, const ASTConstant<U> &);
};

template <typename U>
std::ostream &operator<<(std::ostream &, const ASTConstant<U> &);

} /* namespace by::ast */

#endif /* SRC_AST_ASTCONSTANT_HPP_ */
