/*
 * ASTRoot.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include <algorithm>
#include <ostream>

#include "ast/base.hpp"
#include "ast/extern.hpp"
#include "ast/function.hpp"
#include "peglib.h"
#include "root.hpp"

namespace by::ast {

ASTRoot::ASTRoot(const std::shared_ptr<peg::Ast> &ast) : ASTBase(ast, nullptr) {
  for (const auto &node : ast->nodes) {
    if (node->original_name == "Function") {
      functions.push_back(std::make_shared<ASTFunction>(node));
    } else {
      externs.push_back(std::make_shared<ASTExtern>(node));
    }
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

void ASTRoot::determine_type(type::variable_map &symbols) {

  for (const auto &func : externs) {
    func->determine_type(symbols);
  }
  for (const auto &func : functions) {
    func->determine_type(symbols);
  }
}

void ASTRoot::reorder_functions(const std::vector<std::string> &order) {

  std::vector<std::shared_ptr<by::ast::ASTFunction>> new_functions;
  for (const std::string &func_name : order) {
    auto func =
        std::find_if(functions.begin(), functions.end(),
                     [&](std::shared_ptr<by::ast::ASTFunction> const &f) {
                       return f->get_name() == func_name;
                     });
    if (func != functions.end()) {
      new_functions.push_back((*func));
    }
  }
  functions = new_functions;
}

auto ASTRoot::get_functions()
    -> const std::vector<std::shared_ptr<by::ast::ASTFunction>> & {
  return functions;
}

auto ASTRoot::get_externs()
    -> const std::vector<std::shared_ptr<by::ast::ASTExtern>> & {
  return externs;
}

auto operator<<(std::ostream &os, const ASTRoot &root) -> std::ostream & {
  for (auto &function : root.functions) {
    os << *function << std::endl;
  }
  return os;
};

} /* namespace by::ast */
