/*
 * ASTBlockExpression.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: Manuel Liebchen
 */

#include "block_expression.h"

#include <ext/alloc_traits.h>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "expression.h"
#include "type/type_name.h"
#include "variable_declaration.h"

namespace by::ast {

ASTBlockExpression::ASTBlockExpression(const std::shared_ptr<peg::Ast> &ast,
                                       ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  int n = ast->nodes.size();
  if (ast->nodes.back()->original_name == "EndExpression") {
    end_expression = create_expression(ast->nodes.back()->nodes[0], this);
    n -= 1;
  }
  for (int i = 0; i < n; ++i) {
    expressions.push_back(create_expression(ast->nodes[i], this));
  }
}

ASTBlockExpression::ASTBlockExpression(
    const std::vector<std::shared_ptr<by::ast::ASTVariableDeclaration>>
        &preregistered_variables,
    const std::shared_ptr<peg::Ast> &ast, ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  for (const auto &variable : preregistered_variables) {
    register_variable(variable->get_name(), variable->get_type());
  }
  int n = ast->nodes.size();
  if (ast->nodes.back()->original_name == "EndExpression") {
    end_expression = create_expression(ast->nodes.back()->nodes[0], this);
    n -= 1;
  }
  for (int i = 0; i < n; ++i) {
    expressions.push_back(create_expression(ast->nodes[i], this));
  }
}

auto ASTBlockExpression::determine_type(std::unique_ptr<bc::BuildContext> &bc)
    -> by::type::TypeName_ptr {
  if (*type == *type::TypeName::None) {
    for (auto &exp : expressions) {
      exp->determine_type(bc);
    }

    if (end_expression) {
      type = end_expression->determine_type(bc);
    } else {
      type = type::TypeName::Void;
    }
  }
  return type;
}

auto ASTBlockExpression::find_variable_type(const std::string &name) const
    -> by::type::TypeName_ptr {
  auto iterator = known_variables.find(name);
  if (iterator != known_variables.end()) {
    by::type::TypeName_ptr variable_type = iterator->second;
    if (*variable_type) {
      return variable_type;
    }
  } else if (parent != nullptr) {
    return parent->find_variable_type(name);
  }
  return by::type::TypeName::None;
}

auto ASTBlockExpression::register_variable(const std::string &name,
                                           const by::type::TypeName_ptr &type)
    -> bool {
  auto iterator = known_variables.find(name);
  if (iterator == known_variables.end()) {
    known_variables.emplace(name, type);
    return true;
  }
  return false;
}

auto ASTBlockExpression::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {

  llvm::Value *ret = nullptr;
  for (const auto &exp : expressions) {
    exp->build_ir(bc);
  }

  if (end_expression) {
    ret = end_expression->build_ir(bc);
  }

  return ret;
}

auto ASTBlockExpression::count_generation() const -> size_t {
  if (parent != nullptr) {
    return parent->count_generation() + 1;
  }
  return 1;
}

auto operator<<(std::ostream &os, const ASTBlockExpression &block)
    -> std::ostream & {
  std::string tapstop = "  ";
  std::string prestring;
  size_t generations = block.count_generation();
  for (size_t i = 0; i < generations - 1; ++i) {
    prestring += tapstop;
  }
  os << "{\n";
  for (auto &exp : block.expressions) {
    os << prestring << tapstop << *exp << ";" << std::endl;
  }
  if (block.end_expression) {
    os << prestring << tapstop << *block.end_expression << std::endl;
  }
  os << prestring << "}";
  return os;
}

} /* namespace by::ast */
