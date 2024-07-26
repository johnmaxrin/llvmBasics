#ifndef PACELLVM_H
#define PACELLVM_H

#include <string>
#include <iostream>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"



class Pacellvm {

    public:
        Pacellvm(){
            moduleInit();
            setupExternFunctions();
        }
 
        void run(const std::string &program){

            compile();
            
            module->print(llvm::outs(), nullptr); 
            std::cout<<"\n";

            saveModuleToFile("pacelang.ll");
        }

    private:

        std::unique_ptr<llvm::LLVMContext> context; 
        std::unique_ptr<llvm::Module> module;
        std::unique_ptr<llvm::IRBuilder<>> builder;
        
        // Current Compiling function
        llvm::Function* fn;

        void compile(){
            // i. Create main function:
            fn = createFunction("main", 
                        llvm::FunctionType::get(builder->getInt32Ty(),false));


            // ii. Compile main body.
            // For this lesson, Just return 33.
            llvm::Value *res =  gen();
            builder->CreateRet(res);
        }
        
        llvm::Value* gen()
        { 
            
            auto str = builder->CreateGlobalStringPtr("Hello World\n");
            auto printfFn = module->getFunction("printf");

            std::vector<llvm::Value*> args{str};
            builder->CreateCall(printfFn,args);
            
            llvm::AllocaInst *alloc = builder->CreateAlloca(builder->getInt32Ty(), nullptr, "apple");
            builder->CreateStore(builder->getInt32(33), alloc);
            auto load = builder->CreateLoad(builder->getInt32Ty(), alloc);



            return load;

            //return builder->CreateCall(printfFn,args);

        }


        void setupExternFunctions()
        {
            auto bytePtrTy = builder->getInt8Ty()->getPointerTo();
            module->getOrInsertFunction("printf",
                                        llvm::FunctionType::get(
                                            builder->getInt32Ty(),
                                            bytePtrTy,
                                            true));
        }

        // To create a function we need it's name and type. 
        llvm::Function* createFunction(const std::string& fnName,
                                        llvm::FunctionType* fnType)
        {
            auto fn = module->getFunction(fnName);

            if(fn == nullptr)
                fn = createFunctionProtoType(fnName, fnType);
            
            createFunctionBlock(fn);
            return fn;
        }


        llvm::Function* createFunctionProtoType(const std::string& fnName,
                                        llvm::FunctionType* fnType)
        {
            auto fn = llvm::Function::Create(fnType, 
                            llvm::Function::ExternalLinkage,
                            fnName, *module);
            verifyFunction(*fn);
            return fn;

        }

        void createFunctionBlock(llvm::Function* fn)
        {
            auto entry = createBB("entry", fn);
            builder->SetInsertPoint(entry);
        }

        llvm::BasicBlock* createBB(const std::string name,
                                 llvm::Function* fn=nullptr)
        {
            return llvm::BasicBlock::Create(*context, name,fn);
        }

        void moduleInit(){
            context = std::make_unique<llvm::LLVMContext>();
            module = std::make_unique<llvm::Module>("pacelang", *context);
            builder = std::make_unique<llvm::IRBuilder<>>(*context);
        }

        void saveModuleToFile(const std::string &filename){
            std::error_code EC;
            llvm::raw_fd_ostream dest(filename, EC);
            if (EC) {
                llvm::errs() << "Could not open file: " << EC.message();
                return;
            }
            module->print(dest, nullptr);
        }

};

#endif