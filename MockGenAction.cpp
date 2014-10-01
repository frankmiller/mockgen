
#include "MockGenAction.h"

#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>

class MockGenVisitor : public clang::RecursiveASTVisitor<MockGenVisitor>
{
public:
    MockGenVisitor(clang::SourceManager & sm, std::ostream & os)
        : _sm(sm)
        , _os(os)
    {
    }

    bool VisitCXXRecordDecl(clang::CXXRecordDecl * pClass)
    {
        clang::SourceLocation loc = pClass->getLocation();

        // only examine classes in the file given.
        // TODO will need something different here when I figure a way to not
        // pase -xc++ on the command line.
        if ( !_sm.isInMainFile(loc))
            return true;

        // don't bother with forward declarations
        if ( !pClass->hasDefinition())
            return true;

        std::string name = pClass->getDeclName().getAsString();
        _os << "\nMOCK_BASE_CLASS(Mock" << name << ", " << name << ")\n{\n";

        for (clang::CXXMethodDecl * pFunc : pClass->methods())
        {
            if (pFunc->isVirtual())
            {
                // function name
                std::string funcName = pFunc->getNameInfo().getName().getAsString();

                if (funcName.empty() || funcName[0] == '~')
                    continue;

                // return type
                //std::string returnType = pFunc->getReturnType().getAsString();

                //_os << "    " << returnType << " " << funcName << "(";

                //bool doComma = false;
                //for (clang::ParmVarDecl * pParam : pFunc->params())
                //{
                //    if (doComma)
                //        _os << ", ";
                //    else
                //        doComma = true;
                //    _os << pParam->getType().getAsString() << " " << pParam->getNameAsString();
                //}
                //_os << ");" << std::endl;

                _os << "    MOCK_METHOD(" << funcName << ", " << pFunc->getNumParams() << ");\n";
            }
        }

        _os << "};\n" << std::flush;

        return true;
    }

private:
    clang::SourceManager & _sm;
    std::ostream & _os;
};

class MockGenConsumer : public clang::ASTConsumer
{
public:
    MockGenConsumer(clang::CompilerInstance & ci, std::ostream & os)
        : _visitor(ci.getSourceManager(), os)
    {
    }

    virtual bool HandleTopLevelDecl(clang::DeclGroupRef dgr)
    {
        for (clang::Decl * b : dgr)
            _visitor.TraverseDecl(b);
        return true;
    }

private:
    MockGenVisitor _visitor;
};

std::unique_ptr<clang::ASTConsumer> MockGenAction::CreateASTConsumer(clang::CompilerInstance & ci, llvm::StringRef)
{
    return std::make_unique<MockGenConsumer>(ci, _os);
}
