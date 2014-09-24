
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>

#include <ostream>

class MockGenAction : public clang::ASTFrontendAction
{
public:
    MockGenAction(std::ostream & os)
        : _os(os)
    {
    }

protected:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef) override;

private:
    std::ostream & _os;
};

class MockGenActionFactory : public clang::tooling::FrontendActionFactory
{
public:
    MockGenActionFactory(std::ostream & os)
        : _os(os)
    {
    }

    clang::FrontendAction * create() override { return new MockGenAction(_os); }

private:
    std::ostream & _os;
};

