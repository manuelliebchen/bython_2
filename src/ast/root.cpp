/*
 * ASTRoot.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "root.h"

#include <algorithm>
#include <bits/exception.h>
#include <cstdlib>
#include <iostream>

#include <llvm/IR/Module.h>
#include <llvm/Support/raw_os_ostream.h>

#include "../bc/build_context.h"
#include "extern.h"
#include "function.h"
#include "peglib.h"

namespace by::ast {

ASTRoot::ASTRoot(const std::string &file, const std::shared_ptr<peg::Ast> &ast)
    : ASTRoot(file, ast, std::make_shared<std::unordered_set<std::string>>()) {}

ASTRoot::ASTRoot(
    const std::string &file, const std::shared_ptr<peg::Ast> &ast,
    const std::shared_ptr<std::unordered_set<std::string>> &file_list)
    : file(file), ast(ast), file_list(file_list) {
  file_list->emplace(file);
  for (const auto &node : ast->nodes) {
    if (node->original_name == "Function") {
      functions.push_back(std::make_shared<ASTFunction>(node));
    } else if (node->original_name == "Extern") {
      externs.push_back(std::make_shared<ASTExtern>(node));
    } else if (node->original_name == "Import") {
      imports.push_back(std::make_shared<ASTImport>(node, file_list));
    }
  }
}

void ASTRoot::insert_functions(
    std::unique_ptr<by::bc::BuildContext> &build_context) const {
  for (const auto &import : imports) {
    import->insert_functions(build_context);
  }
  for (const auto &func : externs) {
    build_context->push_back_call(func->get_name(), func->get_function_type());
  }
  for (const auto &func : functions) {
    func->insertFunction(build_context);
  }
}

void ASTRoot::compile(std::ostream &out) {
  std::string last_op;
  bc::BuildContext_ptr build_context =
      std::make_unique<by::bc::BuildContext>(file);
  try {
    last_op = "Declaring Functions";
    insert_functions(build_context);

    last_op = "Determiining return types";
    for (const auto &func : functions) {
      func->determine_type(build_context);
    }
    build_context->module.setTargetTriple("x86_64-pc-linux-gnu");

    last_op = "Compiling";
    for (const auto &func : functions) {
      func->build_ir(build_context);
    }

    last_op = "Wrinting IRCode";
    llvm::raw_os_ostream rso(out);
    build_context->module.print(rso, nullptr);
  } catch (const std::exception &e) {
    std::cerr << "Compilation failed while " << last_op << std::endl;
    std::cerr << e.what() << std::endl;
    exit(1);
  }
}

auto operator<<(std::ostream &os, const ASTRoot &root) -> std::ostream & {
  for (auto &function : root.functions) {
    os << *function << std::endl;
  }
  return os;
};

} /* namespace by::ast */
