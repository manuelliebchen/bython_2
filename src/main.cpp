
#include <exception>
#include <iostream>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_os_ostream.h>
#include <memory>

#include "bc/build_context.hpp"
#include "cxxopts.hpp"
#include "peglib.h"
#include "utillib.hpp"
#include <ast/root.hpp>

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
    const std::string bython_grammar = by::util::read_file(GRAMMAR_PATH);
    peg::parser parser;
    parser.load_grammar(bython_grammar.c_str());
    std::cerr << "Success!\n";

    // Parsing and generating peg ast
    std::cerr << "Parsing Code: ";
    std::string file_path = result["input"].as<std::string>();

    const std::string bython_code = by::util::read_file(file_path);
    parser.enable_ast();
    std::shared_ptr<peg::Ast> basic;
    parser.parse(bython_code.c_str(), basic, file_path.c_str());
    std::cerr << "Success!\n";

    // Building internal AST
    std::shared_ptr<by::ast::ASTRoot> root;
    try {
      std::cerr << "Constructing AST!\n";
      root = std::make_shared<by::ast::ASTRoot>(basic);
      std::cerr << "Success!\n";
    } catch (const by::ast::bad_ast_exeption &bae) {
      std::cerr << bae.what() << std::endl;
      return 1;
    }

    // Calculating compiling order
    std::cerr << "Generate compiling Order: ";
    std::vector<std::string> order = by::util::compiling_order(root);
    root->reorder_functions(order);
    std::cerr << "Success!\n";

    std::cerr << "Determiining return types: ";
    auto build_context = std::make_unique<by::bc::BuildContext>();
    build_context->symbols = by::util::get_buildin_functions();

    try {
      root->determine_type(build_context->symbols);
    } catch (const by::type::type_deduction_exeption &tde) {
      std::cerr << tde.what() << std::endl;
      return 1;
    }
    std::cerr << "Success!\n";

    std::cerr << "Compiling:\n";
    for (const auto &func : root->get_functions()) {
      func->build_ir(build_context);
    }
    std::cerr << "Success!\n";

    std::cerr << "Wrinting Module!\n";
    llvm::raw_os_ostream rso(std::cout);
    build_context->module.print(rso, nullptr);
    std::cerr << "Success!\n";
  } catch (const std::exception &e) {
    std::cerr << "Build failed:\n";
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  return 0;
}
