#include "llvm/IR/PassManager.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include<vector>

using namespace llvm;
using namespace std;
using namespace llvm::PatternMatch;

int found(vector<StringRef> arg, StringRef val){
  int flag = -1;
  for (int i=0; i<arg.size(); i++){
    if(arg[i].compare(val) == 0) flag = i;
  }
  if (flag == -1){
    return -1;
  } else{
    return flag;
  }
}

namespace {
class PropagateIntegerEquality : public PassInfoMixin<PropagateIntegerEquality> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    DominatorTree &DT = FAM.getResult<DominatorTreeAnalysis>(F);

    vector<StringRef> arg;

    for (Argument &Arg : F.args()){
      arg.push_back(Arg.getName());
    }

    for (BasicBlock &BBhead : F){ // for each basicblock starting from top
      BranchInst *TI = dyn_cast<BranchInst>(BBhead.getTerminator());
      if (!TI){
      }else{
        for (auto &I : BBhead){
        Value *X, *Y;
        ConstantInt *C;
        ICmpInst::Predicate Pred;

        // if any other register is found, append to arg
        if (found(arg, I.getName())<0){
          arg.push_back(I.getName());
        }

        if (match(&I, m_ICmp(Pred, m_Value(X), m_Value(Y))) && Pred == ICmpInst::ICMP_EQ) { 
          //if icmp_eq is found in a block's instruction
          outs() << "I found " << I << "\n";

          BasicBlock *BBNext = TI->getSuccessor(0); // only check left branch
          BasicBlockEdge BBE(&BBhead, BBNext); // prepare BBE from BBhead to BBNext, only on left branch
       //   outs() << "head = ///" << BBhead.getName() << " next = " << BBNext->getName() << "\n";
          //check every edge applicable


          // get order of operations

          Value *A = I.getOperand(0);
          Value *B = I.getOperand(1);

          int x = found(arg, A->getName()); 
          int y = found(arg, B->getName()); 

          if (x == -1) {
            arg.push_back(A->getName());
            x = arg.size();
          }
          if (y==-1){
            arg.push_back(B->getName());
            y = arg.size();
          }
        //  int x=0;
         // int y=1;

          if (x<y){
            for (Use &U : Y->uses()){ // locate where Y are used so they can later be replaced by X
           //   outs()   << X->getName() << "\n";
            //  outs() << Y->getName() << "\n";
              User *Usr = U.getUser();
              Instruction *UsrI = dyn_cast<Instruction>(Usr);
              if (UsrI){
                BasicBlock *BBParent = UsrI->getParent(); // spot where (BB) Y was used
              //  outs() << "\t usage found in " << BBParent->getName() << "\n";
                
                if (DT.dominates(BBE, BBParent)){ // if BB is dominated by BBE, replacement can be done
                  if (U != X){
                    outs() << "\t" << "BBedge between " << BBhead.getName() << " and " << BBNext->getName() << " dominates " << BBParent->getName() << "\n";
                    U.set(X);
                  } 
                }else{
                  outs() << "=========" << BBhead.getName() << " -  " << BBNext->getName() << " FAILS " << BBParent->getName() << "\n";
                }
              
              } 
            }
          } else{
         //   outs() << "---------------\n";
            for (Use &U : X->uses()){ // locate where Y are used so they can later be replaced by X
              User *Usr = U.getUser();
              Instruction *UsrI = dyn_cast<Instruction>(Usr);
              if (UsrI){
                BasicBlock *BBParent = UsrI->getParent(); // spot where (BB) Y was used
              //  outs() << "\t usage found in " << BBParent->getName() << "\n";
                
                if (DT.dominates(BBE, BBParent)) // if BB is dominated by BBE, replacement can be done
                  if (U != Y){
                    outs() << "\t" << "BBedge between " << BBhead.getName() << " and " << BBNext->getName() << " dominates " << BBParent->getName() << "\n";
                    U.set(Y);
                }
              } 
            }
          }
        } 
      }
      }
    }
    return PreservedAnalyses::all();
  }
};
}

extern "C" ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "PropagateIntegerEquality", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "prop-int-eq") {
            FPM.addPass(PropagateIntegerEquality());
            return true;
          }
          return false;
        }
      );
    }
  };
}
