/*
 * function_build.cpp
 *
 *  Created on: Mar 18, 2020
 *      Author: jemand
 */

#include "function_build.h"
namespace by::bc {

FunctionBuilder::FunctionBuilder(std::string name, type::FunctionType_ptr type,
                                 build_functional functional)
    : name(name), type(type), functional(functional) {}

FunctionBuilder::FunctionBuilder(std::string name,
                                 const type::FunctionType &type,
                                 build_functional functional)
    : name(name), type(std::make_shared<const type::FunctionType>(type)),
      functional(functional) {}

auto FunctionBuilder::build_ir(std::unique_ptr<BuildContext> &bc,
                               std::vector<llvm::Value *> parameter) const
    -> llvm::Value * {
  return functional(bc, parameter);
}
} // namespace by::bc
