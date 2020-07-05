#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include<string>
#include<vector>
#include<list>
using namespace llvm;
using namespace std;

//void loop(vector<StringRef> strlist, BasicBlock BB);

bool found(vector<StringRef> strlist, StringRef name){
  int flag = 0;
  for (int i=0; i<strlist.size(); i++){
    if(strlist[i].compare(name) == 0) flag = 1;
  }
  if (flag == 1){
    return true;
  } else{
    return false;
  }
}

void loop(vector<StringRef> &strlist, BasicBlock *BB){
  unsigned successorCnt = BB->getTerminator() -> getNumSuccessors();
  for (unsigned i=0; i<successorCnt; i++){
    BasicBlock *NextBB = BB->getTerminator() ->getSuccessor(i);
    if (found(strlist, NextBB->getName())){
    //  outs() << "repeated = " << NextBB->getName() << "\n";
      return;
    }else{
    //  outs() << "inserting = " << NextBB->getName() << "\n";
      strlist.push_back(NextBB->getName());
      loop(strlist, NextBB);
    }
  }
}

namespace {
class MyUnreachablePass : public PassInfoMixin<MyUnreachablePass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {

    vector<StringRef> strlist;
    vector<StringRef> ans;

    int cnt = 0;

    for(BasicBlock &BB : F){ // include all branch
      unsigned successorCnt = BB.getTerminator() -> getNumSuccessors();
      
      for (unsigned i=0; i<successorCnt; i++){
        BasicBlock *NextBB = BB.getTerminator() ->getSuccessor(i);
        strlist.push_back(NextBB->getName());
        loop(strlist, NextBB);
      }
      break;
    }

    int root = 0;
    for(BasicBlock &BB : F){
      if (root == 0){
        root++;
        continue;
      }
      StringRef s = BB.getName();
      int flag = 0;
      for (int i=0; i<strlist.size(); i++){
        if(strlist[i].compare(s) == 0) flag = 1;
      }
      if (flag == 0) {
        ans.push_back(s);
      }
    }  

    std::sort(ans.begin(), ans.end());

    for (int i=0; i<ans.size(); i++){
      outs() << ans[i] << "\n";
    }
    return PreservedAnalyses::all();
  }
};
}

extern "C" ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "MyUnreachablePass", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "my-unreachable") {
            FPM.addPass(MyUnreachablePass());
            return true;
          }
          return false;
        }
      );
    }
  };
}
