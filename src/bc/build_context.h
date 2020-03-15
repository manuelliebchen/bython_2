/*
 * BuildContext.hpp
 *
 *  Created on: Aug 8, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_BUILDCONTEXT_HPP_
#define SRC_AST_BUILDCONTEXT_HPP_

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "../ast/expression.h"
#include "../type/function_type.h"
#include "../type/type_name.h"

namespace by {
namespace ast {
class ASTExpression;
}
namespace bc {

struct BuildContext {
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder;
  llvm::Module module;
  llvm::DataLayout data_layout;

  std::vector<std::unordered_map<std::string, llvm::Value *>> variables;
  std::stack<const ast::ASTExpression *> ast_stack;

  type::variable_map symbols;
  std::unordered_set<type::TypeName> known_types;

  BuildContext(std::string);

};

llvm::Value* build_internal_call(std::unique_ptr<by::bc::BuildContext> &, std::string, std::vector<llvm::Value*>);

} // namespace bc
} // namespace by

#endif /* SRC_AST_BUILDCONTEXT_HPP_ */
