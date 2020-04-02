/*
 * build_context.cpp
 *
 *  Created on: Mar 31, 2020
 *      Author: Manuel Liebchen
 */

#include "function_manager.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/ValueSymbolTable.h>

#include "bc/build_context.h"
#include "bc/function_build.h"
#include "type/function_type.h"
#include "type/type_name.h"

namespace llvm {
class Type;
class Value;
} // namespace llvm

namespace by::bc {

FunctionManager::FunctionManager(BuildContext *bc) : bc(bc) {}

void FunctionManager::emplace_back(const std::string &name,
                                   FunctionPriority prio,
                                   const type::FunctionType_ptr &type,
                                   const build_functional &build) {
  FunctionBuilder fb(name, prio, type, build);
  switch (prio) {
  case FunctionPriority::VARIABLE:
    variables.emplace(name, fb);
    break;
  case FunctionPriority::CAST:
    casts.emplace_back(fb);
    break;
  case FunctionPriority::CALL:
    calls.emplace(name, fb);
    break;
  default:
    int op_type = prio - FunctionPriority::OPERATOR_DOT;
    operators[op_type].emplace(name, fb);
  }
}

auto FunctionManager::get_operators() const
    -> std::vector<std::multimap<std::string, FunctionBuilder>> {
  return operators;
}

auto FunctionManager::find(const std::string &name,
                           const std::vector<type::TypeName_ptr> &type) const
    -> const FunctionBuilder & {
  if (!name.empty()) {
    {
      auto res = variables.equal_range(name);
      for (auto i = res.first; i != res.second; ++i) {
        if (i->second.get_type()->param_equal(type)) {
          return i->second;
        }
      }
    }
    {
      auto res = calls.equal_range(name);
      for (auto i = res.first; i != res.second; ++i) {
        if (i->second.get_type()->param_equal(type)) {
          return i->second;
        }
      }
    }
    for (auto &operator_class : operators) {
      auto res = operator_class.equal_range(name);
      for (auto i = res.first; i != res.second; ++i) {
        if (i->second.get_type()->param_equal(type)) {
          return i->second;
        }
      }
    }
  } else {
    for (const auto &funk : casts) {
      if (funk.get_type()->param_equal(type)) {
        return funk;
      }
    }
  }

  std::string types;
  for (auto &ty : type) {
    types += std::to_string(*ty) + ", ";
  }
  throw std::runtime_error("Could not find function: " + name + " with type " +
                           types);
}

auto FunctionManager::get_type(
    const std::string &name, const std::vector<type::TypeName_ptr> &type) const
    -> type::TypeName_ptr {
  return find(name, type).get_type()->return_type;
}

auto FunctionManager::build(std::unique_ptr<bc::BuildContext> &bc,
                            const std::string &name,
                            const std::vector<type::TypeName_ptr> &type,
                            std::vector<llvm::Value *> parameter) const
    -> llvm::Value * {
  return find(name, type).build_ir(bc, std::move(parameter));
}

void FunctionManager::remove_variable(const std::string &name) {
  auto res = variables.equal_range(name);
  variables.erase(res.first, res.second);
}

void FunctionManager::push_back_call(const std::string &name,
                                     const type::FunctionType_ptr &type) {

  bc->module.getOrInsertFunction(name,
                                 type->get_llvm_function_type(bc->context));

  std::vector<llvm::Type *> llvm_parameters;
  for (auto &para_type : type->parameters) {
    llvm_parameters.emplace_back(para_type->get_llvm_type(bc->context));
  }
  llvm::FunctionCallee function_callee = bc->module.getOrInsertFunction(
      name,
      llvm::FunctionType::get(type->return_type->get_llvm_type(bc->context),
                              llvm_parameters, false));
  emplace_back(
      name, FunctionPriority::CALL, type,
      [=](BuildContext_ptr &bc,
          const std::vector<llvm::Value *> &parameters) -> llvm::Value * {
        return bc->builder.CreateCall(function_callee, parameters);
      });
}

void FunctionManager::push_back_load(const std::string &name,
                                     const type::TypeName_ptr &type) {
  emplace_back(
      name, FunctionPriority::VARIABLE,
      std::make_shared<const type::FunctionType>(type),
      [=](BuildContext_ptr &bc, const std::vector<llvm::Value *> &
          /* unused */) -> llvm::Value * {
        return bc->builder.CreateLoad(
            bc->builder.GetInsertBlock()->getValueSymbolTable()->lookup(
                {name}));
      });
}

auto operator<<(std::ostream &os, const FunctionManager &fm) -> std::ostream & {
  for (const auto &funk : fm.variables) {
    os << funk.second << std::endl;
  }
  for (const auto &funk : fm.calls) {
    os << funk.second << std::endl;
  }
  for (const auto &funk : fm.casts) {
    os << funk << std::endl;
  }
  for (const auto &operator_type : fm.operators) {
    for (const auto &funk : operator_type) {
      os << funk.second << std::endl;
    }
  }
  return os;
}

} // namespace by::bc
