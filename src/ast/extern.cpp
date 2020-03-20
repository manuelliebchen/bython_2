/*
 * ASTFunction.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "extern.h"

#include <ostream>
#include <vector>

#include "../type/function_type.h"
#include "../type/type_name.h"
#include "expression.h"
#include "peglib.h"

namespace by {
namespace bc {
class BuildContext;
} // namespace bc
} // namespace by

namespace by::ast {

ASTExtern::ASTExtern(const std::shared_ptr<peg::Ast> &ast)
    : ASTExpression(ast, nullptr) {
  name = std::to_string(ast->nodes[0]);
  function_type = std::make_shared<const type::FunctionType>(ast->nodes[1]);
  type = function_type->return_type;
}

void ASTExtern::get_dependencies(std::unordered_set<std::string> &functions,
                                 std::unordered_set<std::string> &types) const {
  if (*type) {
    types.insert(std::to_string(*type));
  }
}

auto ASTExtern::get_name() const -> std::string { return name; }

auto ASTExtern::determine_type(std::unique_ptr<bc::BuildContext> &bc)
    -> type::TypeName_ptr {
  bc->symbols.emplace(name, type);
  return type;
}

auto ASTExtern::build_ir(std::unique_ptr<by::bc::BuildContext> &bc) const
    -> llvm::Value * {
  return nullptr;
}

auto operator<<(std::ostream &os, const ASTExtern &func) -> std::ostream & {
  os << "extern func " << func.name << " = ";
  os << " -> " << *func.type << " ";
  return os;
};

} // namespace by::ast
