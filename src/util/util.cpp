/*
 * utillib.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include <algorithm> // for reverse
#include <fstream>
#include <iostream> // for operator<<, ifstream, basic_ostream, cerr
#include <iterator> // for istreambuf_iterator, operator!=
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility> // for pair

#include "ast/base.hpp"       // for ast_error
#include "ast/extern.hpp"     // for ASTExtern
#include "ast/function.hpp"   // for ASTFunction
#include "ast/root.hpp"       // for ASTRoot
#include "peglib.h"           // for AstBase, Ast
#include "type/type_name.hpp" // for variable_map
#include "util.hpp"

namespace by {
namespace type {
struct FunctionType;
}

namespace util {

auto get_buildin_functions() -> type::variable_map {
  std::unordered_map<std::string, type::FunctionType> buildins;
  //  buildins.insert({{"println_int"}, {{"Void"}, {{"Int"}}}});
  //  buildins.insert({{"print_int"}, {{"Void"}, {{"Int"}}}});
  //  buildins.insert({{"println_float"}, {{"Void"}, {{"Float"}}}});
  //  buildins.insert({{"print_float"}, {{"Void"}, {{"Float"}}}});
  //  buildins.insert({{"println_str"}, {{"Void"}, {{"String"}}}});
  //  buildins.insert({{"print_str"}, {{"Void"}, {{"String"}}}});
  //
  //  buildins.insert({{"list_int_init"}, {{"List*"}, {}}});
  //  buildins.insert({{"list_float_init"}, {{"List*"}, {}}});
  //  buildins.insert({{"list_string_init"}, {{"List*"}, {}}});
  //  buildins.insert({{"list_merge"}, {{"List*"}, {{"List*"}, {"List*"}}}});
  //  buildins.insert({{"list_pop_front"}, {{"List*"}, {{"List*"}}}});
  //  buildins.insert({{"list_print"}, {{"Void"}, {{"List*"}}}});
  //  buildins.insert({{"list_size"}, {{"Int"}, {{"List*"}}}});
  //  buildins.insert({{"list_push_back"}, {{"List*"}, {{"List*"},
  //  {"Void*"}}}}); buildins.insert({{"list_get"}, {{"Void*"}, {{"List*"},
  //  {"Int"}}}}); buildins.insert({{"list_get_int"}, {{"Int"}, {{"List*"},
  //  {"Int"}}}}); buildins.insert({{"list_set"}, {{"List*"}, {{"List*"},
  //  {"Int"}, {"Void*"}}}});
  //
  //  buildins.insert({{"iabs"}, {{"Int"}, {{"Int"}}}});
  type::variable_map symbols;
  //  for (const auto &func : buildins) {
  //    symbols.emplace(func.first,
  //                    std::make_shared<const
  //                    type::FunctionType>(func.second));
  //  }

  return symbols;
};

auto compiling_order(const std::shared_ptr<by::ast::ASTRoot> &root)
    -> std::vector<std::string> {
  std::vector<std::string> order;
  std::unordered_map<std::string, std::unordered_set<std::string>> functions;
  for (const auto &ext : root->get_externs()) {
    functions.emplace(ext->get_name(), std::unordered_set<std::string>());
  }
  for (const auto &func : root->get_functions()) {
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
  return order;
};

auto to_string(const std::shared_ptr<peg::Ast> &ast) -> std::string {
  if (ast->original_name == "Identifier" && ast->is_token) {
    return ast->token;
  }
  throw by::ast::ast_error(ast, "Expected Identifier");
};

auto read_file(const std::string &filepath) -> std::string {
  std::ifstream ifs;
  std::cerr << "\nOpening ifstream: " << filepath << " ";
  ifs.open(filepath);
  if (!ifs.good()) {
    throw std::invalid_argument{"Unable to open " + filepath};
  }
  std::cerr << "Success!\n";

  std::string file_string((std::istreambuf_iterator<char>(ifs)),
                          std::istreambuf_iterator<char>());

  ifs.close();

  return file_string;
};

} // namespace util
} // namespace by
