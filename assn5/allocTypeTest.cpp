#include "llvm/AsmParser/Parser.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/SourceMgr.h"
#include "gtest/gtest.h"
#include "allocType.h"

using namespace llvm;
using namespace std;

TEST(AllocTypeTest, MyTest) {
  // Create an IR Module.
  LLVMContext Context;
  unique_ptr<Module> M(new Module("MyTestModule", Context));
  //create module, named myTestModule
  auto *TestFTy = FunctionType::get(Type::getVoidTy(Context), {}, false);
    //create FunctionType with return type void. 
    //does not take argument, no variadic (false)
  auto *MallocFTy = FunctionType::get(Type::getInt8PtrTy(Context),
                                      { Type::getInt64Ty(Context) }, false);
  Function *TestF = Function::Create(TestFTy, Function::ExternalLinkage,
                                     "test", *M);
    //create Create function called test 
    //of function type TestFTy, belongs to M module                               
  Function *MallocF = Function::Create(MallocFTy, Function::ExternalLinkage,
                                       "malloc", *M);

  // Please fill @test() using IRBuilder to TestF.
  // The created program should contain at least:
  //   - One call instruction to malloc
  //     (hint: 'CreateCall(MallocF, { (size argument) }, "p")').
  //   - One alloca instruction
  //   - One bitcast
  //   - One getelementptr

  BasicBlock *Entry = BasicBlock::Create(Context, "entry", TestF);
  IRBuilder<> EntryBuilder(Entry);


  Value *Val = ConstantInt::get(Context, APInt(32, 5));
  Constant* AllocSize = ConstantExpr::getSizeOf(Type::getInt32Ty(Context));


  auto *p = EntryBuilder.CreateCall(MallocF, AllocSize, "p");
  auto *q = EntryBuilder.CreateAlloca(Type::getInt32PtrTy(Context), nullptr, "q");
  auto *b = EntryBuilder.CreateBitCast(Val, Type::getInt32Ty(Context), "b");
  auto *gep = EntryBuilder.CreateGEP(q, ConstantInt::get(Type::getInt32Ty(Context), 1));

  BasicBlock *Exit = BasicBlock::Create(Context, "exit", TestF);
  EntryBuilder.CreateBr(Exit);
  IRBuilder<>(Exit).CreateRetVoid();

  EXPECT_EQ(getBlockType(p), AllocType::HEAP);
  EXPECT_EQ(getBlockType(q), AllocType::STACK);
  EXPECT_EQ(getBlockType(b), AllocType::UNKNOWN);
  EXPECT_EQ(getBlockType(gep), AllocType::STACK);
  

  //one call to malloc
  //one alloca
  //one bit cast
  //one get element pointer

  //with the four created, i can expect.... heap, stack, unknown. 

  // Please write 4 EXPECT_* calls.
  // ex) EXPECT_EQ(getBlockType(p), AllocType::HEAP);
  // There should be at least:
  //   - One EXPECT_* check to AllocType::HEAP
  //   - One EXPECT_* check to AllocType::STACK
  //   - One EXPECT_* check to AllocType::UNKNOWN

  // A dummy EXPECT_EQ.
  // Please delete this line after you finish your implementation.
  //EXPECT_EQ(1234, 5678);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
