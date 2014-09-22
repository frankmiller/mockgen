
#include <clang/Frontend/FrontendAction.h>

class MockGenAction : public clang::ASTFrontendAction
{
protected:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef) override;
};

