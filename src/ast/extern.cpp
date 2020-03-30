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
#include "bc/build_context.h"
#include "expression.h"
#include "peglib.h"

namespace by::ast {

ASTExtern::ASTExtern(const std::shared_ptr<peg::Ast> &ast) {
  name = std::to_string(ast->nodes[0]);
  function_type = std::make_shared<const type::FunctionType>(ast->nodes[1]);
}

auto ASTExtern::get_name() const -> std::string { return name; }

void ASTExtern::insert_functions(
    std::unique_ptr<by::bc::BuildContext> &bc) const {
  bc->push_back_call(name, function_type);
}

auto operator<<(std::ostream &os, const ASTExtern &func) -> std::ostream & {
  os << "extern func " << func.name << " = ";
  os << " -> " << *func.function_type << " ";
  return os;
};

} // namespace by::ast
