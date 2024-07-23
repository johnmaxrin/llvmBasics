#ifndef PACELLVM_H
#define PACELLVM_H

#include <string>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"


class Pacellvm {

    public:
        Pacellvm(){
            moduleInit();
        }
        
        
        void run(const std::string &program){

            module->print(llvm::outs(), nullptr); 
            saveModuleToFile("pacelang.ll");
        }

    private:

        std::unique_ptr<llvm::LLVMContext> context; 
        std::unique_ptr<llvm::Module> module;
        std::unique_ptr<llvm::IRBuilder<>> builder;

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