/*
 * utillib.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "utillib.hpp"

#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include "../ast/base.hpp"
#include "../ast/root.hpp"
#include "../type/function_type.hpp"
#include "../type/type_name.hpp"

namespace by::util {

auto get_buildin_functions() -> type::function_map {
  type::function_map buildins;
  buildins.insert({{"println_int"}, {{"Void"}, {{"Int"}}}});
  buildins.insert({{"print_int"}, {{"Void"}, {{"Int"}}}});
  buildins.insert({{"println_float"}, {{"Void"}, {{"Float"}}}});
  buildins.insert({{"print_float"}, {{"Void"}, {{"Float"}}}});
  buildins.insert({{"println_str"}, {{"Void"}, {{"String"}}}});
  buildins.insert({{"print_str"}, {{"Void"}, {{"String"}}}});

  buildins.insert({{"list_int_init"}, {{"List*"}, {}}});
  buildins.insert({{"list_float_init"}, {{"List*"}, {}}});
  buildins.insert({{"list_string_init"}, {{"List*"}, {}}});
  buildins.insert({{"list_merge"}, {{"List*"}, {{"List*"}, {"List*"}}}});
  buildins.insert({{"list_pop_front"}, {{"List*"}, {{"List*"}}}});
  buildins.insert({{"list_print"}, {{"Void"}, {{"List*"}}}});
  buildins.insert({{"list_size"}, {{"Int"}, {{"List*"}}}});
  buildins.insert({{"list_push_back"}, {{"List*"}, {{"List*"}, {"void*"}}}});
  buildins.insert({{"list_get"}, {{"void*"}, {{"List*"}, {"Int"}}}});
  buildins.insert({{"list_get_int"}, {{"Int"}, {{"List*"}, {"Int"}}}});
  buildins.insert({{"list_set"}, {{"List*"}, {{"List*"}, {"Int"}, {"void*"}}}});

  buildins.insert({{"iabs"}, {{"Int"}, {{"Int"}}}});
  return buildins;
};

auto compiling_order(const std::shared_ptr<by::ast::ASTRoot> &root)
    -> std::vector<std::string> {
  std::vector<std::string> order;
  std::unordered_map<std::string, std::unordered_set<std::string>> functions;
  for (const auto &func : root->get_functions()) {
    std::unordered_set<std::string> all_dep;
    std::unordered_set<std::string> type_dep;
    func->get_dependencies(all_dep, type_dep);
    functions.insert(std::make_pair(func->get_name(), all_dep));
  }

  std::unordered_map<std::string, std::unordered_set<std::string>> list;
  for (auto &func : functions) {
    if (func.first == "main") {
      list.insert(functions.extract(func.first));
      break;
    }
  }
  if (list.empty()) {
    throw by::ast::bad_ast_exeption(root->get_ast(), "No main function found!");
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
  throw by::ast::bad_ast_exeption(ast, "Identifier");
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

} // namespace by::util
