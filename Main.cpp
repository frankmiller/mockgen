
#include "MockGenAction.h"

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

#include <iostream>

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("mocgen options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static llvm::cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
// static llvm::cl::extrahelp MoreHelp("\nMore help text...");

int main(int argc, const char ** argv)
{
    try
    {
        clang::tooling::CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
        clang::tooling::ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
        MockGenActionFactory factory(std::cout);
        return Tool.run(&factory);
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    return 1;
}
