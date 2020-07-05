define i32 @main(i32 %b, i32 %a) {


; CHECK-LABEL: @main(i32 %b, i32 %a)
; CHECK-NEXT: [[X:%.*]] = add i32 [[A:%.*]], [[B:%.*]]
; CHECK-NEXT: [[COND:%.*]] = icmp sgt i32 [[A]], [[X]]
; CHECK-NEXT: br i1 [[COND]], label [[BB_GREATER:%.*]], label [[BB_LESS:%.*]]

; CHECK: bb_less:
; CHECK-NEXT: br label [[BB_EXIT:%.*]]

; CHECK: bb_greater:
; CHECK-NEXT: call void @f(i32 [[X]], i32 [[B]])
; CHECK-NEXT: [[COND2:%.*]] = icmp eq i32 [[A]], [[X]]
; CHECK-NEXT: br i1 [[COND2]], label [[BB_TRUE:%.*]], label [[BB_EXIT]]

; CHECK: bb_true:
; CHECK-NEXT: call void @f(i32 [[A]], i32 [[B]])
; CHECK-NEXT: br label [[BB_LESS]]

; CHECK: bb_exit:
; CHECK-NEXT: call void @f(i32 [[B]], i32 [[A]])
; CHECK-NEXT: ret i32 [[X]]
;

  %x = add i32 %a, %b
  %cond = icmp sgt i32 %a, %x
  br i1 %cond, label %bb_greater, label %bb_less

bb_less:
  br label %bb_exit

bb_greater:
  call void @f(i32 %x, i32 %b)
  %cond2 = icmp eq i32 %a, %x
  br i1 %cond2, label %bb_true, label %bb_exit

bb_true:
  call void @f(i32 %x, i32 %b)
  br label %bb_less

bb_exit:
  call void @f(i32 %b, i32 %a)
  ret i32 %x
  

}

declare void @f(i32, i32)
