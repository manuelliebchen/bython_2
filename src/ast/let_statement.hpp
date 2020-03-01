/*
 * ASTLetStatement.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_ASTLETSTATEMENT_HPP_
#define SRC_AST_ASTLETSTATEMENT_HPP_

#include <llvm/IR/Value.h>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>

#include "expression.hpp"
#include "peglib.h"
#include "type/type_name.hpp"
#include "variable_declaration.hpp"

namespace by {
namespace ast {
class ASTBlockExpression;
} // namespace ast
namespace bc {
struct BuildContext;
} // namespace bc
} // namespace by
namespace llvm {
class Value;
} // namespace llvm

namespace by::ast {

class ASTLetStatement : public ASTExpression {
public:
  ASTLetStatement(const std::shared_ptr<peg::Ast> &, ASTBlockExpression *);

  by::type::TypeName_ptr determine_type(type::variable_map &);

  llvm::Value *build_ir(std::unique_ptr<bc::BuildContext> &) const;

  void get_dependencies(std::unordered_set<std::string> &functions,
                        std::unordered_set<std::string> &types) const;

private:
  std::string var;
  std::shared_ptr<ASTExpression> value;

  friend std::ostream &operator<<(std::ostream &, const ASTLetStatement &);
};

inline std::ostream &operator<<(std::ostream &os, const ASTLetStatement &let) {
  os << "let " << let.var << " = " << *let.value;
  return os;
}
} /* namespace by::ast */

#endif /* SRC_AST_ASTLETSTATEMENT_HPP_ */
