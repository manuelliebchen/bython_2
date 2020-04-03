/*
 * BuildContext.hpp
 *
 *  Created on: Mar 31, 2020
 *      Author: Manuel Liebchen
 */

#ifndef SRC_AST_FUNCTION_MANAGER_HPP_
#define SRC_AST_FUNCTION_MANAGER_HPP_

#include <vector>
#include <map>
#include <memory>
#include <string>

#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "type/function_type.h"
#include "type/type_name.h"
#include "bc/function_priority.h"
#include "bc/function_build.h"

namespace llvm {
class Value;
}  // namespace llvm

namespace by {
namespace bc {

class BuildContext;

class FunctionManager {
private:
  BuildContext * bc;
  std::multimap<std::string, FunctionBuilder> variables;
  std::vector<FunctionBuilder> casts;
  std::multimap<std::string, FunctionBuilder> unary;
  std::vector<std::multimap<std::string, FunctionBuilder>> operators{{},{},{},{}};
  std::multimap<std::string, FunctionBuilder> calls;

  const FunctionBuilder& find(const std::string&, const std::vector<type::TypeName_ptr>& ) const;

public:

  FunctionManager(BuildContext*);

  std::vector<std::multimap<std::string,FunctionBuilder>> get_operators() const;

  void emplace_back(const std::string&, FunctionPriority, const type::FunctionType_ptr&, const build_functional&);

  type::TypeName_ptr get_type(const std::string&, const std::vector<type::TypeName_ptr>& ) const;
  llvm::Value* build(std::unique_ptr<bc::BuildContext> &, const std::string&, const std::vector<type::TypeName_ptr>&, std::vector<llvm::Value *>) const;

  void push_back_call(const std::string&, const type::FunctionType_ptr&);
  void push_back_load(const std::string&, const type::TypeName_ptr&);
  void remove_variable(const std::string&);

  friend std::ostream &operator<<(std::ostream &, const FunctionManager &);
};

std::ostream &operator<<(std::ostream &, const FunctionManager &);

} // namespace bc
} // namespace by

#endif /* SRC_AST_FUNCTION_MANAGER_HPP_ */
