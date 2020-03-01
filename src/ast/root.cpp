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
