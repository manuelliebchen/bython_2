
#include "ast/root.h"
#include <bits/exception.h>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <peglib.h>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <vector>

auto read_file(const std::string &filepath) -> std::string {
  std::ifstream ifs;
  std::cerr << "\nOpening ifstream: " << filepath << " ";
  ifs.open(filepath);
  if (!ifs.good()) {
    throw std::invalid_argument{"Unable to open " + filepath};
  }
  std::cerr << "Success!\n";

  std::string file_string((std::istreambuf_iterator<char>(ifs)),
                          std::istreambuf_iterator<char>());

  ifs.close();

  return file_string;
};

auto main(int argc, char *argv[]) -> int {
  try {
    cxxopts::Options options(*argv, "Bython Compiler.");
    options.positional_help("input").show_positional_help();
    options.allow_unrecognised_options().add_options()(
        "h,help", "Print this Message")("i,input", "bython file",
                                        cxxopts::value<std::string>());

    options.parse_positional({"input"});
    auto result = options.parse(argc, argv);
    if (result.count("help") != 0U) {
      std::cerr << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    // Parsing Grammar
    std::cerr << "Parsing Grammar: ";
    const std::string bython_grammar = read_file(GRAMMAR_PATH);
    peg::parser parser;
    parser.load_grammar(bython_grammar.c_str());
    std::cerr << "Success!\n";

    // Parsing and generating peg ast
    std::cerr << "Parsing Code: ";
    std::string file_path = result["input"].as<std::string>();

    const std::string bython_code = read_file(file_path);
    parser.enable_ast();
    std::shared_ptr<peg::Ast> ast;
    parser.parse(bython_code.c_str(), ast, file_path.c_str());
    std::cerr << "Success!\n";

    // Building internal AST
    std::shared_ptr<by::ast::ASTRoot> root;
    std::cerr << "Constructing AST!\n";
    root = std::make_shared<by::ast::ASTRoot>(ast);
    std::cerr << "Success!\n";

    root->compile(std::cout);
  } catch (const std::exception &e) {
    std::cerr << "\nBuild failed:\n";
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  return 0;
}
