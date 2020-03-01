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
