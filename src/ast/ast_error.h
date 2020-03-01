
#ifndef __AST_ERROR_h
#define __AST_ERROR_h

#include <peglib.h>
#include <stdexcept>

namespace by::ast {
/**
 * error for all ast related errors, prints position in code
 */
struct ast_error : public std::runtime_error {
  ast_error(const std::shared_ptr<peg::Ast> ast, std::string what_str)
      : runtime_error(ast->path + ":" + std::to_string(ast->line) + ":" +
                      std::to_string(ast->column) + ": " + what_str) {}
};
} // namespace by::ast
#endif //__AST_ERROR_h
