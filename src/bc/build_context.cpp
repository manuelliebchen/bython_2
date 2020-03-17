/*
 * build_context.cpp
 *
 *  Created on: Mar 13, 2020
 *      Author: Manuel Liebchen
 */

#include "build_context.h"

#include <cctype>

namespace by::bc {

BuildContext::BuildContext(std::string name)
    : context(), builder(context), module(name, context), data_layout(&module) {

}

// {":", TypeName::List, {TypeName::List, TypeName::List},
// [](std::vector<llvm::Value*> param)->llvm::Value*{//Stuff}};

llvm::Value *build_internal_call(std::unique_ptr<by::bc::BuildContext> &bc,
                                 std::string name, type::TypeName_ptr type,
                                 std::vector<llvm::Value *> parameter) {
  std::vector<llvm::Type *> llvm_parameters;
  llvm::Type *llvm_returntype = nullptr;
  if (name == "list_peek") {
    std::string type_name = type->name;
    std::transform(type_name.begin(), type_name.end(), type_name.begin(),
                   ::tolower);
    name += "_" + type_name;
    llvm_parameters.emplace_back(
        type::TypeName::List->get_llvm_type(bc->context));
    llvm_returntype = type->get_llvm_type(bc->context);
  } else if (name == "list_push") {
    std::string type_name = type->name;
    std::transform(type_name.begin(), type_name.end(), type_name.begin(),
                   ::tolower);
    name += "_" + type_name;
    llvm_parameters.emplace_back(type->get_llvm_type(bc->context));
    llvm_returntype = type::TypeName::List->get_llvm_type(bc->context);
  } else if (name == "list_pop") {
    llvm_parameters.emplace_back(
        type::TypeName::List->get_llvm_type(bc->context));
    llvm_returntype = type::TypeName::List->get_llvm_type(bc->context);
  } else if (name == "list_concatenate") {
    llvm_parameters.emplace_back(
        type::TypeName::List->get_llvm_type(bc->context));
    llvm_parameters.emplace_back(
        type::TypeName::List->get_llvm_type(bc->context));
    llvm_returntype = type::TypeName::List->get_llvm_type(bc->context);
  } else {
    throw std::runtime_error("Can not find build in function: " + name);
  }

  llvm::FunctionCallee function_callee = bc->module.getOrInsertFunction(
      name, llvm::FunctionType::get(llvm_returntype, llvm_parameters, false));

  return bc->builder.CreateCall(function_callee, parameter);
}

} // namespace by::bc
