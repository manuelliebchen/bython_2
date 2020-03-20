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
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "../ast/expression.h"
#include "../type/function_type.h"
#include "../type/type_name.h"
#include "function_build.h"

namespace by {
namespace ast {
class ASTExpression;
}
namespace bc {
class FunctionBuilder;

class BuildContext : public std::vector<FunctionBuilder> {
public:
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder;
  llvm::Module module;
  llvm::DataLayout data_layout;

  std::vector<std::unordered_map<std::string, llvm::Value *>> variables;
  std::stack<const ast::ASTExpression *> ast_stack;

  type::variable_map symbols;

  BuildContext(std::string);

  const FunctionBuilder& find(std::string, const std::vector<type::TypeName_ptr>& ) const;
  const FunctionBuilder& find(std::string) const;

  void push_back_call(std::string, type::FunctionType_ptr);

private:
  void push_back_call(std::string, type::FunctionType);
  void build_buildin();
};


} // namespace bc
} // namespace by

#endif /* SRC_AST_BUILDCONTEXT_HPP_ */
