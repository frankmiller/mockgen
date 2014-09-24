
#include "MockGenAction.h"

#include <clang/Frontend/FrontendActions.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>

#include <iostream>

class MockGenVisitor : public clang::RecursiveASTVisitor<MockGenVisitor>
{
public:
    MockGenVisitor(std::ostream & os)
        : _os(os)
    {
    }

    bool VisitCXXRecordDecl(clang::CXXRecordDecl * pClass)
    {
        std::string name = pClass->getDeclName().getAsString();
        _os << "class " << name << std::endl;

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

                _os << "    " << returnType << " " << funcName << "(";

                bool doComma = false;
                for (clang::ParmVarDecl * pParam : pFunc->params())
                {
                    if (doComma)
                        _os << ", ";
                    else
                        doComma = true;
                    _os << pParam->getType().getAsString() << " " << pParam->getNameAsString();
                }
                _os << ");" << std::endl;
            }
        }

        return true;
    }

private:
    std::ostream & _os;
};

class MockGenConsumer : public clang::ASTConsumer
{
public:
    MockGenConsumer(std::ostream & os)
        : _visitor(os)
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

std::unique_ptr<clang::ASTConsumer> MockGenAction::CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef)
{
    return std::make_unique<MockGenConsumer>(_os);
}
