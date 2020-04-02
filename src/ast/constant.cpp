/*
 * ASTConstant.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: Manuel Liebchen
 */

#include "constant.h"

#include <utility> // for move

#include "expression.h" // for operator<<, ASTExpression
#include "type/type_name.h"

namespace by {
namespace bc {
class BuildContext;
} // namespace bc

namespace ast {
class ASTBlockExpression;

// Bool
template <>
ASTConstant<bool>::ASTConstant(const std::shared_ptr<peg::Ast> &ast,
                               ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  type = type::TypeName::Bool;
  value = false;
  if (ast->token == "true") {
    value = true;
  }
}
template <>
auto ASTConstant<bool>::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  return value ? llvm::ConstantInt::getTrue(bc->context)
               : llvm::ConstantInt::getFalse(bc->context);
}
template <>
auto operator<<(std::ostream &os, const ASTConstant<bool> &con)
    -> std::ostream & {
  if (con.value) {
    os << "true";
  } else {
    os << "false";
  }
  return os;
}

// Int
template <>
ASTConstant<int>::ASTConstant(const std::shared_ptr<peg::Ast> &ast,
                              ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  type = type::TypeName::Int;
  value = std::stoi(ast->token);
}
template <>
auto ASTConstant<int>::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  auto llvm_int_type =
      llvm::IntegerType::get(bc->context, std::numeric_limits<int>::digits + 1);

  return llvm::ConstantInt::getSigned(llvm_int_type, value);
}
template <>
auto operator<<(std::ostream &os, const ASTConstant<int> &con)
    -> std::ostream & {
  os << std::to_string(con.value);
  return os;
}

// Float
template <>
ASTConstant<float>::ASTConstant(const std::shared_ptr<peg::Ast> &ast,
                                ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  type = type::TypeName::Float;
  value = std::stof(ast->token);
}
template <>
auto ASTConstant<float>::build_ir(std::unique_ptr<bc::BuildContext> &bc) const
    -> llvm::Value * {
  return llvm::ConstantFP::get(bc->context, llvm::APFloat(value));
}
template <>
auto operator<<(std::ostream &os, const ASTConstant<float> &con)
    -> std::ostream & {
  os << std::to_string(con.value);
  return os;
}

// String
template <>
ASTConstant<std::string>::ASTConstant(const std::shared_ptr<peg::Ast> &ast,
                                      ASTBlockExpression *parent)
    : ASTExpression(ast, parent) {
  type = type::TypeName::String;
  value = ast->token;
  auto p = value.find(R"(\n)");
  if (p != std::string::npos) {
    value.replace(p, 2, "\n");
  }
}
template <>
auto ASTConstant<std::string>::build_ir(
    std::unique_ptr<bc::BuildContext> &bc) const -> llvm::Value * {
  const std::string gname = ".str" + value;
  const auto gdata = llvm::ConstantDataArray::getString(bc->context, value,
                                                        /*AddNull*/ true);

  // Register the declaration statement in the module
  // Note: Name string declaration after string content?
  // Note: Would help avoid duplicates?
  llvm::Constant *text = bc->module.getOrInsertGlobal(gname, gdata->getType());
  const auto gdata_loc = llvm::cast<llvm::GlobalVariable>(text);

  gdata_loc->setInitializer(gdata);

  // Cast to appropriate type and return
  return llvm::ConstantExpr::getBitCast(
      gdata_loc, type::TypeName::String->get_llvm_type(bc->context));
}
template <>
auto operator<<(std::ostream &os, const ASTConstant<std::string> &con)
    -> std::ostream & {
  os << "\"" << con.value << "\"";
  return os;
}

template <typename T> auto ASTConstant<T>::get_value() const -> T {
  return value;
}

template <typename T>
auto ASTConstant<T>::determine_type(std::unique_ptr<bc::BuildContext> & /*bc*/)
    -> by::type::TypeName_ptr {
  return type;
}

template class ASTConstant<bool>;
template class ASTConstant<int>;
template class ASTConstant<float>;
template class ASTConstant<std::string>;

} /* namespace ast */
} /* namespace by */
