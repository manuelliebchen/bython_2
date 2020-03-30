/*
 * build_context.cpp
 *
 *  Created on: Mar 13, 2020
 *      Author: Manuel Liebchen
 */

#include "build_context.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

#include <llvm/ADT/StringRef.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/ValueSymbolTable.h>

#include "bc/function_build.h"
#include "type/function_type.h"
#include "type/type_name.h"

namespace llvm {
class Type;
class Value;
} // namespace llvm

namespace by::bc {

BuildContext::BuildContext(const std::string &name)
    : module(name, context), data_layout(&module) {
  build_buildin();
}

auto BuildContext::find(const std::string &name,
                        const std::vector<type::TypeName_ptr> &type) const
    -> const FunctionBuilder & {
  auto func = find_if(begin(), end(), [&](const FunctionBuilder &func) -> bool {
    if (func.get_name() == name) {
      if (func.get_type()->param_equal(type)) {
        return true;
      }
    }
    return false;
  });
  if (func != end()) {
    return *func;
  }

  std::string types;
  for (auto &ty : type) {
    types += std::to_string(*ty) + ", ";
  }
  throw std::runtime_error("Could not find function: " + name + " with type " +
                           types);
}

void BuildContext::remove(const std::string &name) {
  erase(
      std::remove_if(begin(), end(), [&](const FunctionBuilder &func) -> bool {
        return func.get_name() == name;
      }));
}

void BuildContext::push_back_call(const std::string &name,
                                  const type::FunctionType_ptr &type) {

  module.getOrInsertFunction(name, type->get_llvm_function_type(context));

  std::vector<llvm::Type *> llvm_parameters;
  for (auto &para_type : type->parameters) {
    llvm_parameters.emplace_back(para_type->get_llvm_type(context));
  }
  llvm::FunctionCallee function_callee = module.getOrInsertFunction(
      name, llvm::FunctionType::get(type->return_type->get_llvm_type(context),
                                    llvm_parameters, false));
  emplace_back(
      name, 5, type,
      [=](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateCall(function_callee, parameters);
      });
}

void BuildContext::push_back_call(const std::string &name,
                                  const type::FunctionType &type) {
  push_back_call(name, std::make_shared<const type::FunctionType>(type));
}

void BuildContext::push_back_load(const std::string &name,
                                  const type::TypeName_ptr &type) {

  emplace_back(
      name, -1, type,
      [=](BuildContext_ptr &bc, const std::vector<llvm::Value *> &
          /* unused */) -> llvm::Value * {
        return bc->builder.CreateLoad(
            bc->builder.GetInsertBlock()->getValueSymbolTable()->lookup(
                {name}));
      });
}

void BuildContext::build_buildin() {
  emplace_back("Null", -1, type::FunctionType{type::TypeName::Null},
               [&](BuildContext_ptr &bc, const std::vector<llvm::Value *> &
                   /* parameters */) -> llvm::Value * {
                 return llvm::ConstantPointerNull::get(
                     llvm::Type::getInt8PtrTy(bc->context));
               });
  emplace_back(
      "", 0, type::FunctionType{type::TypeName::Float, type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateSIToFP(
            parameters[0], type::TypeName::Float->get_llvm_type(bc->context));
      });
  emplace_back(
      "==", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Int,
                         type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpEQ(parameters[0], parameters[1]);
      });
  emplace_back(
      "==", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Float,
                         type::TypeName::Float},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpOEQ(parameters[0], parameters[1]);
      });
  emplace_back(
      "!=", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Int,
                         type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpNE(parameters[0], parameters[1]);
      });
  emplace_back(
      "!=", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Float,
                         type::TypeName::Float},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpONE(parameters[0], parameters[1]);
      });
  emplace_back(
      ">", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Int,
                         type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpSGT(parameters[0], parameters[1]);
      });
  emplace_back(
      ">", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Float,
                         type::TypeName::Float},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpOGT(parameters[0], parameters[1]);
      });
  emplace_back(
      ">=", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Int,
                         type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpSGE(parameters[0], parameters[1]);
      });
  emplace_back(
      ">=", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Float,
                         type::TypeName::Float},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpOGE(parameters[0], parameters[1]);
      });
  emplace_back(
      "<", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Int,
                         type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpSLT(parameters[0], parameters[1]);
      });
  emplace_back(
      "<", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Float,
                         type::TypeName::Float},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpOLT(parameters[0], parameters[1]);
      });
  emplace_back(
      "<=", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Int,
                         type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpSLE(parameters[0], parameters[1]);
      });
  emplace_back(
      "<=", 3,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Float,
                         type::TypeName::Float},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpOLE(parameters[0], parameters[1]);
      });
  emplace_back(
      "+", 2,
      type::FunctionType{type::TypeName::Int, type::TypeName::Int,
                         type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateAdd(parameters[0], parameters[1]);
      });
  emplace_back(
      "+", 2,
      type::FunctionType{type::TypeName::Float, type::TypeName::Float,
                         type::TypeName::Float},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFAdd(parameters[0], parameters[1]);
      });
  emplace_back(
      "-", 2,
      type::FunctionType{type::TypeName::Int, type::TypeName::Int,
                         type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateSub(parameters[0], parameters[1]);
      });
  emplace_back(
      "-", 2,
      type::FunctionType{type::TypeName::Float, type::TypeName::Float,
                         type::TypeName::Float},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFSub(parameters[0], parameters[1]);
      });
  emplace_back(
      "*", 1,
      type::FunctionType{type::TypeName::Int, type::TypeName::Int,
                         type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateMul(parameters[0], parameters[1]);
      });
  emplace_back(
      "*", 1,
      type::FunctionType{type::TypeName::Float, type::TypeName::Float,
                         type::TypeName::Float},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFMul(parameters[0], parameters[1]);
      });
  emplace_back(
      "/", 1,
      type::FunctionType{type::TypeName::Int, type::TypeName::Int,
                         type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateSDiv(parameters[0], parameters[1]);
      });
  emplace_back(
      "/", 1,
      type::FunctionType{type::TypeName::Float, type::TypeName::Float,
                         type::TypeName::Float},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFDiv(parameters[0], parameters[1]);
      });
  emplace_back(
      "%", 1,
      type::FunctionType{type::TypeName::Int, type::TypeName::Int,
                         type::TypeName::Int},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateSRem(parameters[0], parameters[1]);
      });
  emplace_back(
      "&&", 1,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Bool,
                         type::TypeName::Bool},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateAnd(parameters[0], parameters[1]);
      });
  emplace_back(
      "||", 2,
      type::FunctionType{type::TypeName::Bool, type::TypeName::Bool,
                         type::TypeName::Bool},
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateOr(parameters[0], parameters[1]);
      });
  for (auto &type : type::TypeName::native) {
    build_all_list_operator(type);
  }
  build_all_list_operator(type::TypeName::String);

  push_back_call("b_init", type::FunctionType());
  push_back_call("b_deinit", type::FunctionType());
}

void BuildContext::build_all_list_operator(const type::TypeName_ptr &type) {
  std::string type_name = "record";
  if (type->is_native()) {
    type_name = type->name;
    std::transform(type_name.begin(), type_name.end(), type_name.begin(),
                   ::tolower);
  }

  auto list_type = type::TypeName::make({"List", {type}});

  emplace_back(
      "", 0, type::FunctionType{type::TypeName::Null, list_type},
      [&](BuildContext_ptr & /* bc */,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return parameters[0];
      });

  push_back_call("list_peek_" + type_name, type::FunctionType{type, list_type});
  push_back_call("list_push_" + type_name,
                 type::FunctionType{list_type, type, list_type});
  push_back_call("list_push_" + type_name,
                 type::FunctionType{list_type, type, type::TypeName::Null});
  push_back_call("list_concatenate",
                 type::FunctionType{list_type, list_type, list_type});
  push_back_call("list_concatenate", type::FunctionType{list_type, list_type,
                                                        type::TypeName::Null});

  push_back_call("list_pop", type::FunctionType{list_type, list_type});
  emplace_back(
      ":", 4, type::FunctionType{list_type, type, list_type},
      [=](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->find("list_push_" + type_name, {type, list_type})
            .build_ir(bc, parameters);
      });
  emplace_back(
      ":", 4, type::FunctionType{list_type, type, type::TypeName::Null},
      [=](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->find("list_push_" + type_name, {type, type::TypeName::Null})
            .build_ir(bc, parameters);
      });
  emplace_back(
      ":", 4, type::FunctionType{list_type, list_type, list_type},
      [=](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->find("list_concatenate", {list_type, list_type})
            .build_ir(bc, parameters);
      });
  emplace_back(
      ":", 4, type::FunctionType{list_type, list_type, type::TypeName::Null},
      [=](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->find("list_concatenate", {list_type, type::TypeName::Null})
            .build_ir(bc, parameters);
      });
}

} // namespace by::bc
