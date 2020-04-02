/*
 * BuildContext.hpp
 *
 *  Created on: Aug 8, 2019
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_BUILDCONTEXT_HPP_
#define SRC_AST_BUILDCONTEXT_HPP_

#include <llvm/IR/DataLayout.h>
#include <memory>
#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "type/type_name.h"
#include "bc/function_manager.h"

namespace by {
namespace bc {

class BuildContext {
public:
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder{context};
  llvm::Module module;
  llvm::DataLayout data_layout;

  FunctionManager functions;

  BuildContext(const std::string&);
private:
  void build_buildin();
  void build_all_list_operator(const type::TypeName_ptr&);
};

using BuildContext_ptr = std::unique_ptr<BuildContext>;

} // namespace bc
} // namespace by

#endif /* SRC_AST_BUILDCONTEXT_HPP_ */
