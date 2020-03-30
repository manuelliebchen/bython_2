/*
 * BuildContext.hpp
 *
 *  Created on: Aug 8, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_BUILDCONTEXT_HPP_
#define SRC_AST_BUILDCONTEXT_HPP_

#include <stack>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include <llvm/IR/DataLayout.h>
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ValueSymbolTable.h"

#include "../ast/expression.h"
#include "../type/function_type.h"
#include "../type/type_name.h"
#include "function_build.h"

namespace by {
namespace bc {
class FunctionBuilder;

class BuildContext : public std::vector<FunctionBuilder> {
public:
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder{context};
  llvm::Module module;
  llvm::DataLayout data_layout;

  BuildContext(const std::string&);

  const FunctionBuilder& find(const std::string&, const std::vector<type::TypeName_ptr>& ) const;

  void push_back_call(const std::string&, const type::FunctionType_ptr&);
  void push_back_call(const std::string&, const type::FunctionType&);
  void push_back_load(const std::string&, const type::TypeName_ptr&);
  void remove(const std::string&);

private:
  void build_buildin();
  void build_all_list_operator(const type::TypeName_ptr&);
};

using BuildContext_ptr = std::unique_ptr<BuildContext>;

} // namespace bc
} // namespace by

#endif /* SRC_AST_BUILDCONTEXT_HPP_ */
