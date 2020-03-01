/*
 * utillib.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_UTILLIB_HPP_
#define SRC_UTILLIB_HPP_

#include <memory>
#include <string>
#include <vector>

#include <peglib.h>

#include <type/type_name.hpp>

namespace by {
namespace ast {
class ASTRoot;
}

namespace util {
std::vector<std::string>
compiling_order(const std::shared_ptr<by::ast::ASTRoot> &);

std::string to_string(const std::shared_ptr<peg::Ast> &ast);
std::string read_file(const std::string &filepath);
} // namespace util
} // namespace by

#endif /* SRC_UTILLIB_HPP_ */
