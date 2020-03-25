/*
 * ASTImport.cpp
 *
 *  Created on: Mar 24, 2020
 *      Author: Manuel Liebchen
 */

#include "import.h"

#include <fstream>
#include <ostream>
#include <vector>

#include "../type/function_type.h"
#include "../type/type_name.h"
#include "bc/build_context.h"
#include "expression.h"
#include "peglib.h"
#include "string_constant.h"

auto read_file(const std::string &filepath) -> std::string {
  std::ifstream ifs;
  ifs.open(filepath);
  if (!ifs.good()) {
    throw std::invalid_argument{"Unable to open " + filepath};
  }

  std::string file_string((std::istreambuf_iterator<char>(ifs)),
                          std::istreambuf_iterator<char>());

  ifs.close();

  return file_string;
};

namespace by::ast {

ASTImport::ASTImport(const std::shared_ptr<peg::Ast> &ast)
    : ASTExpression(ast, nullptr) {
  file = ASTStringConstant(ast->nodes[0], nullptr).get_value();
  type = type::TypeName::Void;

  const std::string bython_grammar = read_file(GRAMMAR_PATH);
  peg::parser parser;
  parser.load_grammar(bython_grammar.c_str());
  const std::string bython_code = read_file(file);
  parser.enable_ast();
  std::shared_ptr<peg::Ast> root_ast;
  if (!parser.parse(bython_code.c_str(), root_ast, file.c_str())) {
    peg::Log log = parser.log;
    throw std::runtime_error("Unable to Parse file: Syntax Error in import: " +
                             file);
  }

  // Building internal AST
  root = std::make_shared<ASTRoot>(file, root_ast);
}

auto ASTImport::get_name() const -> std::string { return file; }

void ASTImport::insert_functions(
    std::unique_ptr<by::bc::BuildContext> &bc) const {
  root->insert_functions(bc);
};

auto ASTImport::determine_type(std::unique_ptr<bc::BuildContext> &bc)
    -> type::TypeName_ptr {
  return type::TypeName::Void;
}

auto ASTImport::build_ir(std::unique_ptr<by::bc::BuildContext> &bc) const
    -> llvm::Value * {
  return nullptr;
}

auto operator<<(std::ostream &os, const ASTImport &func) -> std::ostream & {
  os << "import \"" << func.file << "\"";
  return os;
};

} // namespace by::ast
