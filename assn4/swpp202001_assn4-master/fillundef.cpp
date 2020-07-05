#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include<vector>

using namespace llvm;
using namespace std;

// This example shows how to visit uses of an instruction, and selectively
// replace use with UndefValue.
// After it is run, all non-constant operands in "undef_zone" block will be
// replaced with undef.

namespace {
class FillUndef : public PassInfoMixin<FillUndef> {
  void replaceSpecificUsesWithUndef(Value *V) {
    for (Use &U : V->uses()) {
      outs() << "usage = " << U->getName() << "\n";
      User *Usr = U.getUser();
     // outs() << "user = " << Usr->get << "\n";
      Instruction *UsrI = dyn_cast<Instruction>(Usr);
     // outs() << "userI = " << UsrI << "\n";
      if (UsrI) {
        BasicBlock *BB = UsrI->getParent();
        if (BB->getName() == "undef_zone")
        //  outs() << "UNAME = " << U->getName() << "\n";
        //  outs() << "UndefValue::get(UsrI->getType())" << UndefValue::get(UsrI->getType()) << "\n";
          U.set(UndefValue::get(UsrI->getType()));
      }
    }
  }
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    for (Argument &Arg : F.args()){
   //   outs() << Arg << "\n";
      replaceSpecificUsesWithUndef(&Arg);
    }
    outs() << "break\n\n"; 

    for (auto &BB : F){
      StringRef s = BB.getName();
     // outs() << "BB = " << s << "\n";
      for (auto &I : BB){
       // outs() << "instruction name = " <<  I << "\n";
        replaceSpecificUsesWithUndef(&I);
      }
    }
    return PreservedAnalyses::all();
  }
};
}

extern "C" ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "FillUndef", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "fill-undef") {
            FPM.addPass(FillUndef());
            return true;
          }
          return false;
        }
      );
    }
  };
}
