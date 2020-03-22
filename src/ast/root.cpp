/*
 * ASTRoot.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "root.h"

#include <algorithm>
#include <bits/exception.h>
#include <iostream>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_os_ostream.h>
#include <stdlib.h>

#include "../bc/build_context.h"
#include "extern.h"
#include "function.h"
#include "peglib.h"

namespace by::ast {

ASTRoot::ASTRoot(std::string file, const std::shared_ptr<peg::Ast> &ast)
    : file(file), ast(ast) {
  for (const auto &node : ast->nodes) {
    if (node->original_name == "Function") {
      functions.push_back(std::make_shared<ASTFunction>(node));
    } else {
      externs.push_back(std::make_shared<ASTExtern>(node));
    }
  }
}

void ASTRoot::compile(std::ostream &out) {
  std::string last_op = "";
  try {
    bc::BuildContext_ptr build_context =
        std::make_unique<by::bc::BuildContext>(file);
    for (const auto &func : externs) {
      build_context->push_back_call(func->get_name(),
                                    func->get_function_type());
    }
    last_op = "Declaring Functions";
    for (const auto &func : functions) {
      func->insertFunction(build_context);
    }

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
