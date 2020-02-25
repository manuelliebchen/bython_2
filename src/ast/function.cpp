/*
 * ASTFunction.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "function.hpp"

#include <llvm/IR/Function.h>

namespace by::ast {

ASTFunction::ASTFunction(const std::shared_ptr<peg::Ast> &ast,
                         ASTBlockExpression *parent)
    : ASTBase(ast, parent) {
  if (ast->original_name != "Function") {
    throw bad_ast_exeption(
        ast, (std::string("Function but was ") + ast->original_name).c_str());
  }

  size_t varscount = ast->nodes.size() - 2;
  name = util::to_string(ast->nodes[0]);
  if (const std::shared_ptr<peg::Ast> &astreturntype = *(ast->nodes.end() - 2);
      astreturntype->original_name == "TypeName") {
    returntype = std::make_shared<type::TypeName>(astreturntype);
    --varscount;
  } else {
    returntype = std::make_shared<type::TypeName>();
  }
  for (size_t i = 0; i < varscount; ++i) {
    parameters.push_back(
        std::make_shared<ASTVariableDeclaration>(ast->nodes[1 + i], parent));
  }
  blockexpression = std::make_shared<ASTBlockExpression>(
      parameters, ast->nodes.back(), parent);
}

void ASTFunction::determine_type(type::function_map known_functions) {
  std::cerr << std::to_string(get_function_type()) << std::endl;
  known_functions.emplace(name, get_function_type());
  blockexpression->determine_type(known_functions);
}

void ASTFunction::get_dependencies(
    std::unordered_set<std::string> &functions,
    std::unordered_set<std::string> &types) const {
  for (const auto &parameter : parameters) {
    parameter->get_dependencies(functions, types);
  }
  if (returntype) {
    types.insert(type::to_string(*returntype));
  }
  blockexpression->get_dependencies(functions, types);
}

auto ASTFunction::get_name() const -> std::string { return name; }

void ASTFunction::build_ir(std::unique_ptr<by::bc::BuildContext> &bc) const {
  std::cerr << "Building " << name << std::endl;

  type::FunctionType type = get_function_type();

  llvm::Function *function =
      llvm::Function::Create(type.get_llvm_type(bc->context),
                             llvm::Function::ExternalLinkage, name, bc->module);

  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(bc->context, "entry", function);
  bc->builder.SetInsertPoint(BB);

  bc->variables.emplace_back(std::unordered_map<std::string, llvm::Value *>());
  unsigned idx = 0;
  for (auto &arg : function->args()) {
    std::string name = parameters[idx++]->get_name();
    arg.setName(name);
    bc->variables.back().emplace(
        std::make_pair<std::string, llvm::Value *>(std::string(name), &arg));
  }

  if (llvm::Value *return_value = blockexpression->build_ir(bc);
      return_value != nullptr) {
    bc->builder.CreateRet(return_value);
  }

  bc->variables.pop_back();
  bc->known_functions.emplace(name, type);
}

auto ASTFunction::get_function_type() const -> type::FunctionType {
  type::FunctionType sig(*returntype);
  for (auto &para : parameters) {
    sig.parameters.emplace_back(type::TypeName(para->get_type()));
  }
  return sig;
}

auto operator<<(std::ostream &os, const ASTFunction &func) -> std::ostream & {
  os << "func " << func.name << " ";
  if (func.parameters.empty()) {
    os << "Void";
  } else {
    for (auto &para : func.parameters) {
      os << *para << ", ";
    }
  }
  if (func.returntype) {
    os << "-> " << *func.returntype << " ";
  }
  os << *func.blockexpression << "\n";
  return os;
};

} // namespace by::ast
