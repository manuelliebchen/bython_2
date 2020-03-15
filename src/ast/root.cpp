/*
 * ASTRoot.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "root.h"

#include <algorithm>
#include <iostream>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_os_ostream.h>
#include <stdexcept>
#include <unordered_map>
#include <utility>

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
    // Calculating compiling order
    //    last_op = "Generate compiling Order";
    //    reorder_functions();

    last_op = "Determiining return types";
    auto build_context = std::make_unique<by::bc::BuildContext>(file);
    for (const auto &func : externs) {
      func->determine_type(build_context->symbols);
    }
    for (const auto &func : functions) {
      build_context->symbols.emplace(func->get_name(), func->get_type());
    }
    for (const auto &func : functions) {
      func->determine_type(build_context->symbols);
    }
    build_context->module.setTargetTriple("x86_64-pc-linux-gnu");

    last_op = "Declaring Functions";
    for (const auto &func : functions) {
      func->insertFunction(build_context);
    }

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

void ASTRoot::get_dependencies(std::unordered_set<std::string> &functions,
                               std::unordered_set<std::string> &types) const {

  //  for (const auto &func : this->externs) {
  //    func->get_dependencies(functions, types);
  //  }
  for (const auto &func : this->functions) {
    func->get_dependencies(functions, types);
  }
}

void ASTRoot::reorder_functions() {

  std::vector<std::string> order;
  std::unordered_map<std::string, std::unordered_set<std::string>> functions;
  for (const auto &func : this->functions) {
    std::unordered_set<std::string> all_dep;
    std::unordered_set<std::string> type_dep;
    func->get_dependencies(all_dep, type_dep);
    functions.emplace(func->get_name(), all_dep);
  }

  std::unordered_map<std::string, std::unordered_set<std::string>> list;
  for (auto &func : functions) {
    if (func.first == "main") {
      list.insert(functions.extract(func.first));
      break;
    }
  }
  if (list.empty()) {
    throw std::runtime_error("No main function found!");
  }

  while (!list.empty()) {
    auto current = *list.begin();
    list.erase(current.first);
    functions.erase(current.first);
    order.push_back(current.first);
    for (const std::string &dep : current.second) {
      if (dep == current.first) {
        continue;
      }
      bool add = true;
      for (auto func : functions) {
        if (dep == func.first) {
          continue;
        }
        add &= func.second.find(dep) == func.second.end();
      }
      if (add) {
        if (auto it = functions.find(dep); it != functions.end()) {
          list.insert(*it);
        }
      }
    }
  }
  std::reverse(order.begin(), order.end());

  std::vector<std::shared_ptr<by::ast::ASTFunction>> new_functions;
  for (const std::string &func_name : order) {
    ;
    auto func =
        std::find_if(this->functions.begin(), this->functions.end(),
                     [&](std::shared_ptr<by::ast::ASTFunction> const &f) {
                       return f->get_name() == func_name;
                     });
    if (func != this->functions.end()) {
      new_functions.push_back(*func);
    }
  }
  this->functions = new_functions;
}

auto operator<<(std::ostream &os, const ASTRoot &root) -> std::ostream & {
  for (auto &function : root.functions) {
    os << *function << std::endl;
  }
  return os;
};

} /* namespace by::ast */
