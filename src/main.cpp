
#include <iostream>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_os_ostream.h>
#include <memory>

#include "bc/build_context.hpp"
#include "cxxopts.hpp"
#include "peglib.h"
#include "utillib.hpp"
#include <ast/root.hpp>

int main(int argc, char *argv[]) {
  try {
    cxxopts::Options options(argv[0], "Bython Compiler.");
    options.positional_help("input").show_positional_help();
    options.allow_unrecognised_options().add_options()(
        "h,help", "Print this Message")("i,input", "bython file",
                                        cxxopts::value<std::string>());
    options.parse_positional({"input"});
    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      std::cerr << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    std::cerr << "Parsing Grammar: ";
    const std::string bython_grammar = by::util::read_file(GRAMMAR_PATH);
    peg::parser parser;
    parser.load_grammar(bython_grammar.c_str());
    std::cerr << "Success!\n";

    std::cerr << "Parsing Code: ";
    const std::string bython_code =
        by::util::read_file(result["input"].as<std::string>());
    parser.enable_ast();
    std::shared_ptr<peg::Ast> basic;
    parser.parse(bython_code.c_str(), basic);
    std::cerr << "Success!\n";

    std::cerr << "Constructing AST!\n";
    std::shared_ptr<by::ast::ASTRoot> root =
        std::make_shared<by::ast::ASTRoot>(basic, nullptr);
    std::cerr << "Success!\n";

    std::cerr << "Generate compiling Order: ";
    std::vector<std::string> order = by::util::compiling_order(root);
    root->reorder_functions(order);
    std::cerr << "Success!\n";

    std::cerr << "Determiining return types: ";
    auto build_context = std::make_unique<by::bc::BuildContext>();
    build_context->known_functions = by::util::get_buildin_functions();

    for (const auto &func : root->get_functions()) {
      build_context->known_functions.emplace(func->get_name(),
                                             func->get_function_type());
    }

    const auto &functions = root->get_functions();
    for (const auto &func : functions) {
      func->determine_type(build_context->known_functions);
    }
    std::cerr << "Success!\n";

    std::cerr << "Compiling:\n";
    for (const auto &func : functions) {
      func->build_ir(build_context);
    }
    std::cerr << "Success!\n";

    std::cerr << "Wrinting Module!\n";
    llvm::raw_os_ostream rso(std::cout);
    build_context->module.print(rso, nullptr);
    std::cerr << "Success!\n";
  } catch (const cxxopts::OptionException &e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }
  return 0;
}
