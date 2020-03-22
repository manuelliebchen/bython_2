/*
 * function_build.cpp
 *
 *  Created on: Mar 18, 2020
 *      Author: jemand
 */

#include "function_build.h"

#include "type/function_type.h"

namespace llvm {
class Value;
} // namespace llvm

namespace by::bc {

FunctionBuilder::FunctionBuilder(std::string name, int priority,
                                 type::FunctionType_ptr type,
                                 build_functional functional)
    : name(std::move(name)), priority(priority), type(std::move(type)),
      functional(std::move(functional)) {}

FunctionBuilder::FunctionBuilder(std::string name, int priority,
                                 const type::FunctionType &type,
                                 build_functional functional)
    : name(std::move(name)), priority(priority),
      type(std::make_shared<const type::FunctionType>(type)),
      functional(std::move(functional)) {}

auto FunctionBuilder::build_ir(std::unique_ptr<BuildContext> &bc,
                               std::vector<llvm::Value *> parameter) const
    -> llvm::Value * {
  return functional(bc, std::move(parameter));
}
} // namespace by::bc
