/*
 * ASTRoot.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "root.hpp"

namespace by::ast {

ASTRoot::ASTRoot(const std::shared_ptr<peg::Ast> &ast) : ASTBase(ast, nullptr) {
  for (const auto &node : ast->nodes) {
    functions.push_back(std::make_shared<ASTFunction>(node, parent));
  }
}

void ASTRoot::get_dependencies(std::unordered_set<std::string> &functions,
                               std::unordered_set<std::string> &types) const {
  for (const auto &func : this->functions) {
    func->get_dependencies(functions, types);
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
    new_functions.push_back((*func));
  }
  functions = new_functions;
}

auto ASTRoot::get_functions()
    -> const std::vector<std::shared_ptr<by::ast::ASTFunction>> & {
  return functions;
}

auto operator<<(std::ostream &os, const ASTRoot &root) -> std::ostream & {
  for (auto &function : root.functions) {
    os << *function << std::endl;
  }
  return os;
};

} /* namespace by::ast */
