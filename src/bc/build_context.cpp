/*
 * build_context.cpp
 *
 *  Created on: Mar 13, 2020
 *      Author: Manuel Liebchen
 */

#include "build_context.h"

#include <algorithm>
#include <cctype>
#include <vector>

#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>

#include "bc/function_priority.h"
#include "function_manager.h"
#include "type/function_type.h"
#include "type/type_name.h"

namespace llvm {
class Value;
} // namespace llvm

namespace by::bc {

BuildContext::BuildContext(const std::string &name)
    : module(name, context), data_layout(&module), functions(this) {
  build_buildin();
}

void BuildContext::build_buildin() {
  functions.emplace_back(
      "Null", FunctionPriority::VARIABLE,
      std::make_shared<const type::FunctionType>(type::TypeName::Null),
      [&](BuildContext_ptr &bc, const std::vector<llvm::Value *> &
          /* parameters */) -> llvm::Value * {
        return llvm::ConstantPointerNull::get(
            llvm::Type::getInt8PtrTy(bc->context));
      });
  functions.emplace_back(
      "-", FunctionPriority::UNARY,
      std::make_shared<const type::FunctionType>(type::TypeName::Int,
                                                 type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateNeg(parameters.front());
      });
  functions.emplace_back(
      "-", FunctionPriority::UNARY,
      std::make_shared<const type::FunctionType>(type::TypeName::Float,
                                                 type::TypeName::Float),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFNeg(parameters.front());
      });
  functions.emplace_back(
      "!", FunctionPriority::UNARY,
      std::make_shared<const type::FunctionType>(type::TypeName::Bool,
                                                 type::TypeName::Bool),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateNot(parameters.front());
      });
  functions.emplace_back(
      "", FunctionPriority::CAST,
      std::make_shared<const type::FunctionType>(type::TypeName::Float,
                                                 type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateSIToFP(
            parameters[0], type::TypeName::Float->get_llvm_type(bc->context));
      });
  functions.emplace_back(
      "==", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Int, type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpEQ(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "==", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Float, type::TypeName::Float),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpOEQ(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "!=", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Int, type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpNE(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "!=", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Float, type::TypeName::Float),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpONE(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      ">", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Int, type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpSGT(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      ">", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Float, type::TypeName::Float),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpOGT(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      ">=", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Int, type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpSGE(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      ">=", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Float, type::TypeName::Float),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpOGE(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "<", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Int, type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpSLT(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "<", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Float, type::TypeName::Float),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpOLT(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "<=", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Int, type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateICmpSLE(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "<=", FunctionPriority::OPERATOR_COMPARE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Float, type::TypeName::Float),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFCmpOLE(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "+", FunctionPriority::OPERATOR_LINE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Int, type::TypeName::Int, type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateAdd(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "+", FunctionPriority::OPERATOR_LINE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Float, type::TypeName::Float, type::TypeName::Float),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFAdd(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "-", FunctionPriority::OPERATOR_LINE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Int, type::TypeName::Int, type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateSub(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "-", FunctionPriority::OPERATOR_LINE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Float, type::TypeName::Float, type::TypeName::Float),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFSub(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "*", FunctionPriority::OPERATOR_DOT,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Int, type::TypeName::Int, type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateMul(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "*", FunctionPriority::OPERATOR_DOT,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Float, type::TypeName::Float, type::TypeName::Float),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFMul(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "/", FunctionPriority::OPERATOR_DOT,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Int, type::TypeName::Int, type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateSDiv(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "/", FunctionPriority::OPERATOR_DOT,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Float, type::TypeName::Float, type::TypeName::Float),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateFDiv(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "%", FunctionPriority::OPERATOR_DOT,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Int, type::TypeName::Int, type::TypeName::Int),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateSRem(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "&&", FunctionPriority::OPERATOR_DOT,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Bool, type::TypeName::Bool),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateAnd(parameters[0], parameters[1]);
      });
  functions.emplace_back(
      "||", FunctionPriority::OPERATOR_LINE,
      std::make_shared<const type::FunctionType>(
          type::TypeName::Bool, type::TypeName::Bool, type::TypeName::Bool),
      [&](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateOr(parameters[0], parameters[1]);
      });
  for (auto &type : type::TypeName::native) {
    build_all_list_operator(type);
  }
  build_all_list_operator(type::TypeName::String);

  functions.push_back_call("b_init",
                           std::make_shared<const type::FunctionType>());
  functions.push_back_call("b_deinit",
                           std::make_shared<const type::FunctionType>());
}

void BuildContext::build_all_list_operator(const type::TypeName_ptr &type) {
  std::string type_name = "record";
  if (type->is_native()) {
    type_name = type->name;
    std::transform(type_name.begin(), type_name.end(), type_name.begin(),
                   ::tolower);
  }

  auto list_type = type::TypeName::make({"List", {type}});

  functions.emplace_back(
      "", FunctionPriority::CAST,
      std::make_shared<const type::FunctionType>(type::TypeName::Null,
                                                 list_type),
      [&](BuildContext_ptr & /* bc */,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return parameters[0];
      });

  functions.push_back_call(
      "list_peek_" + type_name,
      std::make_shared<const type::FunctionType>(type, list_type));
  functions.push_back_call(
      "list_push_" + type_name,
      std::make_shared<const type::FunctionType>(list_type, type, list_type));
  functions.push_back_call("list_concatenate",
                           std::make_shared<const type::FunctionType>(
                               list_type, list_type, list_type));

  functions.push_back_call(
      "list_pop",
      std::make_shared<const type::FunctionType>(list_type, list_type));
  functions.emplace_back(
      ":", FunctionPriority::OPERATOR_LIST,
      std::make_shared<const type::FunctionType>(list_type, type, list_type),
      [=](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->functions.build(bc, "list_push_" + type_name,
                                   {type, list_type}, parameters);
      });
  functions.emplace_back(
      ":", FunctionPriority::OPERATOR_LIST,
      std::make_shared<const type::FunctionType>(list_type, list_type,
                                                 list_type),
      [=](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->functions.build(bc, "list_concatenate",
                                   {list_type, list_type}, parameters);
      });
}

} // namespace by::bc
