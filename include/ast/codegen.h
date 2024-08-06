#ifndef PACECODEGEN
#define PACECODEGEN

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"

#include "visitor.h"
#include <stdio.h>

class Codegen : public Visitor
{
public:
    Codegen() : builder(context), module("pacelang", context)
    {
        auto bytePtrTy = builder.getInt8Ty()->getPointerTo();
        module.getOrInsertFunction("printf",
                                    llvm::FunctionType::get(
                                        builder.getInt32Ty(),
                                        bytePtrTy,
                                        true));

        llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getInt32Ty(), false);
        mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);

        // Create an entry basic block
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
        builder.SetInsertPoint(entry);
    }

    void VisitExpr(const Expression *expr)
    {

        llvm::Value *result;

        if (expr->lhs == nullptr)
        {
            printf("Here\n");
        }
        else
        {

            if (expr->op == '+')
            {

                llvm::Value *lhsvar = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "a");
                llvm::Value *rhsvar = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "b");
                llvm::Value *resvar = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "res");

                builder.CreateStore(builder.getInt32(expr->lhs->val), lhsvar);
                builder.CreateStore(builder.getInt32(expr->rhs->val), rhsvar);

                llvm::Value *load_lhs = builder.CreateLoad(builder.getInt32Ty(), lhsvar, "lhs_load");
                llvm::Value *load_rhs = builder.CreateLoad(builder.getInt32Ty(), rhsvar, "rhs_load");

                llvm::Value *sum = builder.CreateAdd(load_lhs, load_rhs, "sum");
                builder.CreateStore(sum, resvar);
                auto printfFn = module.getFunction("printf");
                std::vector<llvm::Value*> args{resvar};
                builder.CreateCall(printfFn,args);
            }
        }

        builder.CreateRet(builder.getInt32(0));
        llvm::verifyFunction(*mainFunc);
    }

    llvm::Module *getModule()
    {
        return &module;
    }

    llvm::IRBuilder<> &getBuilder()
    {
        return builder;
    }

    void generateCode(const char *filename)
    {
        std::error_code EC;
        llvm::raw_fd_ostream dest(filename, EC);
        if (EC)
        {
            llvm::errs() << "Could not open file: " << EC.message();
            return;
        }
        getModule()->print(dest, nullptr);
    }

private:
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    llvm::Module module;
    llvm::Function *mainFunc;
};

#endif

// TODO:

/*
    1. Pass LLVM Context, IRBuilder, Module
    2. Generate LLVM Code for Expressions
*/