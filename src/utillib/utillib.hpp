/*
 * utillib.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_UTILLIB_HPP_
#define SRC_UTILLIB_HPP_

#include <memory>
#include <peglib.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "../type/function_type.hpp"
#include "../type/type_name.hpp"

#include "../ast/base.hpp"
#include "../ast/root.hpp"

namespace by::ast {
class ASTRoot;
}

namespace by::util {
type::variable_map get_buildin_functions();
std::vector<std::string>
compiling_order(const std::shared_ptr<by::ast::ASTRoot> &);

std::string to_string(const std::shared_ptr<peg::Ast> &ast);
std::string read_file(const std::string &filepath);
} // namespace by::util

#endif /* SRC_UTILLIB_HPP_ */
