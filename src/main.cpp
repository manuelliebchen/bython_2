
#include <iostream>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_os_ostream.h>
#include <memory>

#include "ast/root.hpp"
#include "bc/build_context.hpp"
#include "cxxopts.hpp"
#include "peglib.h"
#include "utillib.hpp"

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
    std::cerr << "Success!\n";

    std::cerr << "Compiling:\n";
    auto build_context = std::make_unique<by::bc::BuildContext>();
    build_context->known_functions = by::util::get_buildin_functions();
    const auto &functions = root->get_functions();
    for (const std::string &func_name : order) {
      auto func =
          std::find_if(functions.begin(), functions.end(),
                       [&](std::shared_ptr<by::ast::ASTFunction> const &f) {
                         return f->get_name() == func_name;
                       });
      (*func)->build_ir(build_context);
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
