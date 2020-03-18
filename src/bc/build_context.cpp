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
    : vector(), context(), builder(context), module(name, context),
      data_layout(&module) {
  build_buildin();
}

// {":", TypeName::List, {TypeName::List, TypeName::List},
// [](std::vector<llvm::Value*> param)->llvm::Value*{//Stuff}};

auto BuildContext::build_internal_call(std::unique_ptr<BuildContext> &bc,
                                       std::string name,
                                       type::TypeName_ptr type,
                                       std::vector<llvm::Value *> parameter)
    -> llvm::Value * {
  //  std::vector<llvm::Type *> llvm_parameters;
  //  llvm::Type *llvm_returntype = nullptr;
  //  if (name == "list_peek") {
  //    std::string type_name = type->name;
  //    std::transform(type_name.begin(), type_name.end(), type_name.begin(),
  //                   ::tolower);
  //    name += "_" + type_name;
  //    llvm_parameters.emplace_back(type::TypeName::List->get_llvm_type(context));
  //    llvm_returntype = type->get_llvm_type(context);
  //  } else if (name == "list_push") {
  //    std::string type_name = type->name;
  //    std::transform(type_name.begin(), type_name.end(), type_name.begin(),
  //                   ::tolower);
  //    name += "_" + type_name;
  //    llvm_parameters.emplace_back(type->get_llvm_type(context));
  //    llvm_returntype = type::TypeName::List->get_llvm_type(context);
  //  } else if (name == "list_pop") {
  //    llvm_parameters.emplace_back(type::TypeName::List->get_llvm_type(context));
  //    llvm_returntype = type::TypeName::List->get_llvm_type(context);
  //  } else if (name == "list_concatenate") {
  //    llvm_parameters.emplace_back(type::TypeName::List->get_llvm_type(context));
  //    llvm_parameters.emplace_back(type::TypeName::List->get_llvm_type(context));
  //    llvm_returntype = type::TypeName::List->get_llvm_type(context);
  //  } else {
  //    throw std::runtime_error("Can not find build in function: " + name);
  //  }
  //
  //  llvm::FunctionCallee function_callee = module.getOrInsertFunction(
  //      name, llvm::FunctionType::get(llvm_returntype, llvm_parameters,
  //      false));
  //
  //  return builder.CreateCall(function_callee, parameter);

  auto &builder = find(name);
  return builder.build_ir(bc, parameter);
}

auto BuildContext::find(std::string name, type::FunctionType_ptr type) const
    -> const FunctionBuilder & {
  auto func = find_if(begin(), end(), [&](const FunctionBuilder &func) -> bool {
    return func.get_name() == name && *func.get_type() == *type;
  });
  if (func != end()) {
    return *func;
  }
  throw std::runtime_error("Could not find function: " + name + " with type " +
                           std::to_string(*type));
}

auto BuildContext::find(std::string name) const -> const FunctionBuilder & {
  auto func = find_if(begin(), end(), [&](const FunctionBuilder &func) -> bool {
    return func.get_name() == name;
  });
  if (func != end()) {
    return *func;
  }
  throw std::runtime_error("Could not find function: " + name);
}

void BuildContext::build_buildin() {
  emplace_back(
      "", type::FunctionType{type::TypeName::Float, type::TypeName::Int},
      [&](std::unique_ptr<BuildContext> &bc,
          std::vector<llvm::Value *> parameters) -> llvm::Value * {
        return bc->builder.CreateSIToFP(
            parameters[0], type::TypeName::Float->get_llvm_type(bc->context));
      });
  emplace_back("==",
               type::FunctionType{type::TypeName::Bool, type::TypeName::Int,
                                  type::TypeName::Int},
               [&](std::unique_ptr<BuildContext> &bc,
                   std::vector<llvm::Value *> parameters) -> llvm::Value * {
                 return bc->builder.CreateICmpEQ(parameters[0], parameters[1]);
               });
  emplace_back("==",
               type::FunctionType{type::TypeName::Bool, type::TypeName::Float,
                                  type::TypeName::Float},
               [&](std::unique_ptr<BuildContext> &bc,
                   std::vector<llvm::Value *> parameters) -> llvm::Value * {
                 return bc->builder.CreateFCmpOEQ(parameters[0], parameters[1]);
               });
  emplace_back("!=",
               type::FunctionType{type::TypeName::Bool, type::TypeName::Int,
                                  type::TypeName::Int},
               [&](std::unique_ptr<BuildContext> &bc,
                   std::vector<llvm::Value *> parameters) -> llvm::Value * {
                 return bc->builder.CreateICmpNE(parameters[0], parameters[1]);
               });
  emplace_back("!=",
               type::FunctionType{type::TypeName::Bool, type::TypeName::Float,
                                  type::TypeName::Float},
               [&](std::unique_ptr<BuildContext> &bc,
                   std::vector<llvm::Value *> parameters) -> llvm::Value * {
                 return bc->builder.CreateFCmpONE(parameters[0], parameters[1]);
               });
  emplace_back(">",
               type::FunctionType{type::TypeName::Bool, type::TypeName::Int,
                                  type::TypeName::Int},
               [&](std::unique_ptr<BuildContext> &bc,
                   std::vector<llvm::Value *> parameters) -> llvm::Value * {
                 return bc->builder.CreateICmpSGT(parameters[0], parameters[1]);
               });
  emplace_back(">",
               type::FunctionType{type::TypeName::Bool, type::TypeName::Float,
                                  type::TypeName::Float},
               [&](std::unique_ptr<BuildContext> &bc,
                   std::vector<llvm::Value *> parameters) -> llvm::Value * {
                 return bc->builder.CreateFCmpOGT(parameters[0], parameters[1]);
               });
  push_back_call("list_pop", type::FunctionType{type::TypeName::List,
                                                type::TypeName::List});
  push_back_call("list_concatenate",
                 type::FunctionType{type::TypeName::List, type::TypeName::List,
                                    type::TypeName::List});
  push_back_call("list_peek_int",
                 type::FunctionType{type::TypeName::Int, type::TypeName::List});
  push_back_call("list_peek_float", type::FunctionType{type::TypeName::Float,
                                                       type::TypeName::List});
  push_back_call("list_peek_bool", type::FunctionType{type::TypeName::Bool,
                                                      type::TypeName::List});
  push_back_call("list_peek_string", type::FunctionType{type::TypeName::String,
                                                        type::TypeName::List});
  push_back_call("list_push_int",
                 type::FunctionType{type::TypeName::List, type::TypeName::Int,
                                    type::TypeName::List});
  push_back_call("list_push_float",
                 type::FunctionType{type::TypeName::List, type::TypeName::Float,
                                    type::TypeName::List});
  push_back_call("list_push_bool",
                 type::FunctionType{type::TypeName::List, type::TypeName::Bool,
                                    type::TypeName::List});
  push_back_call("list_push_string", type::FunctionType{type::TypeName::List,
                                                        type::TypeName::String,
                                                        type::TypeName::List});
}

void BuildContext::push_back_call(std::string name, type::FunctionType type) {
  std::vector<llvm::Type *> llvm_parameters;
  for (auto &para_type : type.parameters) {
    llvm_parameters.emplace_back(para_type->get_llvm_type(context));
  }
  llvm::FunctionCallee function_callee = module.getOrInsertFunction(
      name, llvm::FunctionType::get(type.return_type->get_llvm_type(context),
                                    llvm_parameters, false));
  emplace_back(name, type,
               [=](std::unique_ptr<BuildContext> &bc,
                   std::vector<llvm::Value *> parameters) -> llvm::Value * {
                 return bc->builder.CreateCall(function_callee, parameters);
               });
}

} // namespace by::bc
