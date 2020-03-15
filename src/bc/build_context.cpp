/*
 * build_context.cpp
 *
 *  Created on: Mar 13, 2020
 *      Author: Manuel Liebchen
 */

#include "build_context.h"

namespace by::bc {

BuildContext::BuildContext(std::string name)
    : context(), builder(context), module(name, context), data_layout(&module) {

}

llvm::Value *build_internal_call(std::unique_ptr<by::bc::BuildContext> &bc,
                                 std::string name,
                                 std::vector<llvm::Value *> parameter) {
  std::vector<llvm::Type *> llvm_parameters;
  llvm::Type *llvm_returntype = nullptr;
  if (name == "list_init_main") {
    llvm_parameters.emplace_back(
        type::TypeName::Int->get_llvm_type(bc->context));
    llvm_parameters.emplace_back(
        llvm::Type::getInt8PtrTy(bc->context)->getPointerTo());
    llvm_returntype = type::TypeName::List->get_llvm_type(bc->context);
  } else if (name == "list_peek") {
    llvm_parameters.emplace_back(
        type::TypeName::List->get_llvm_type(bc->context));
    llvm_returntype = llvm::Type::getInt8PtrTy(bc->context)->getPointerTo();
  } else if (name == "list_pop") {
    llvm_parameters.emplace_back(
        type::TypeName::List->get_llvm_type(bc->context));
    llvm_returntype = type::TypeName::List->get_llvm_type(bc->context);
  } else {
    throw std::runtime_error("Can not find build in functino: " + name);
  }

  llvm::FunctionCallee function_callee = bc->module.getOrInsertFunction(
      name, llvm::FunctionType::get(llvm_returntype, llvm_parameters, false));

  return bc->builder.CreateCall(function_callee, parameter);
}

} // namespace by::bc
