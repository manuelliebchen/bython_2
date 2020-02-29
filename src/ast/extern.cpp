/*
 * ASTFunction.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "extern.hpp"

#include <llvm/IR/Function.h>

namespace by::ast {

ASTExtern::ASTExtern(const std::shared_ptr<peg::Ast> &ast)
    : ASTExpression(ast, nullptr) {
  if (ast->original_name != "Extern") {
    throw bad_ast_exeption(
        ast, (std::string("Extern but was ") + ast->original_name).c_str());
  }
  name = util::to_string(ast->nodes[0]);
  type = std::make_shared<const type::FunctionType>(ast->nodes[1]);
}

void ASTExtern::get_dependencies(std::unordered_set<std::string> &functions,
                                 std::unordered_set<std::string> &types) const {
  if (*type) {
    types.insert(std::to_string(*type));
  }
}

auto ASTExtern::get_name() const -> std::string { return name; }

auto ASTExtern::determine_type(type::variable_map &symbol)
    -> type::TypeName_ptr {
  symbol.emplace(name, type);
  return type;
}

auto ASTExtern::build_ir(std::unique_ptr<by::bc::BuildContext> &bc) const
    -> llvm::Value * {
  return nullptr;
}

auto operator<<(std::ostream &os, const ASTExtern &func) -> std::ostream & {
  os << "func " << func.name << " = ";
  os << "-> " << *func.type << " ";
  return os;
};

} // namespace by::ast
