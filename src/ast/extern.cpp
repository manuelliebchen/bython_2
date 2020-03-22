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

auto ASTExtern::get_name() const -> std::string { return name; }

auto ASTExtern::determine_type(std::unique_ptr<bc::BuildContext> &bc)
    -> type::TypeName_ptr {
  bc->push_back_call(name, function_type);
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
