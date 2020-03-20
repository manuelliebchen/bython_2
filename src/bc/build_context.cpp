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

auto BuildContext::find(std::string name,
                        const std::vector<type::TypeName_ptr> &type) const
    -> const FunctionBuilder & {
  auto func = find_if(begin(), end(), [&](const FunctionBuilder &func) -> bool {
    return func.get_name() == name && func.get_type()->param_equal(type);
  });
  if (func != end()) {
    return *func;
  }
  throw std::runtime_error("Could not find function: " + name);
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

void BuildContext::push_back_call(std::string name,
                                  type::FunctionType_ptr type) {

  symbols.emplace(name, type->return_type);
  module.getOrInsertFunction(name, type->get_llvm_function_type(context));

  std::vector<llvm::Type *> llvm_parameters;
  for (auto &para_type : type->parameters) {
    llvm_parameters.emplace_back(para_type->get_llvm_type(context));
  }
  llvm::FunctionCallee function_callee = module.getOrInsertFunction(
      name, llvm::FunctionType::get(type->return_type->get_llvm_type(context),
                                    llvm_parameters, false));
  emplace_back(name, type,
               [=](std::unique_ptr<BuildContext> &bc,
                   std::vector<llvm::Value *> parameters) -> llvm::Value * {
                 return bc->builder.CreateCall(function_callee, parameters);
               });
}

void BuildContext::push_back_call(std::string name, type::FunctionType type) {
  push_back_call(name, std::make_shared<const type::FunctionType>(type));
}

} // namespace by::bc
