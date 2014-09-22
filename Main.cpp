
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"

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

class MockGenVisitor : public clang::RecursiveASTVisitor<MockGenVisitor>
{
public:
    MockGenVisitor() {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl * pClass)
    {
        std::string name = pClass->getDeclName().getAsString();
        std::cout << "class " << name << std::endl;

        for (clang::CXXMethodDecl * pFunc : pClass->methods())
        {
            if (pFunc->isVirtual())
            {
                // function name
                std::string funcName = pFunc->getNameInfo().getName().getAsString();

                if (funcName.empty() || funcName[0] == '~')
                    continue;

                // return type
                std::string returnType = pFunc->getReturnType().getAsString();

                std::cout << "    " << returnType << " " << funcName << "(";

                bool doComma = false;
                for (clang::ParmVarDecl * pParam : pFunc->params())
                {
                    if (doComma)
                        std::cout << ", ";
                    else
                        doComma = true;
                    std::cout << pParam->getType().getAsString() << " " << pParam->getNameAsString();
                }
                std::cout << ");" << std::endl;
            }
        }

        return true;
    }
};

class MockGenConsumer : public clang::ASTConsumer
{
public:
    virtual bool HandleTopLevelDecl(clang::DeclGroupRef dgr)
    {
        for (clang::Decl * b : dgr)
            _visitor.TraverseDecl(b);
        return true;
    }

private:
    MockGenVisitor _visitor;
};

class MocGenAction : public clang::ASTFrontendAction
{
protected:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef) override
    {
        return std::make_unique<MockGenConsumer>();
    }
};

int main(int argc, const char ** argv)
{
    clang::tooling::CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    clang::tooling::ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
    return Tool.run(clang::tooling::newFrontendActionFactory<MocGenAction>().get());
}
