define i32 @main(i32 %a, i32 %b, i32 %c) { 

; CHECK-LABEL: @main(i32 %a, i32 %b, i32 %c)
; CHECK-NEXT: [[COND:%.*]] = icmp eq i32 [[A:%.*]], [[C:%.*]]
; CHECK-NEXT: br i1 [[COND]], label [[BB_TRUE:%.*]], label [[BB_FALSE:%.*]]

; CHECK: bb_true:
; CHECK-NEXT: call void @f(i32 [[A]], i32 [[B:%.*]], i32 [[A]])
; CHECK-NEXT: [[X:%.*]] = add i32 [[A]], [[B]]
; CHECK-NEXT: [[Y:%.*]] = mul i32 [[B]], [[A]]
; CHECK-NEXT: [[COND2:%.*]] = icmp eq i32 [[B]], [[Y]]
; CHECK-NEXT: call void @f(i32 [[A]], i32 [[B]], i32 [[A]])
; CHECK-NEXT: br i1 [[COND2]], label [[BB_TRUE2:%.*]], label [[BB_FALSE]]

; CHECK: bb_true2:
; CHECK-NEXT: call void @f(i32 [[A]], i32 [[B]], i32 [[A]])
; CHECK-NEXT: [[COND3:%.*]] = icmp eq i32 [[X]], [[A]]
; CHECK-NEXT: br label [[BB_EXIT:%.*]]

; CHECK: bb_false:
; CHECK-NEXT: br label [[BB_EXIT2:%.*]]

; CHECK: bb_exit:
; CHECK-NEXT: call void @f(i32 [[A]], i32 [[B]], i32 [[A]])
; CHECK-NEXT: ret i32 [[B]]

; CHECK: bb_exit2:
; CHECK-NEXT: ret i32 [[B]]
;
  %cond = icmp eq i32 %a, %c
  br i1 %cond, label %bb_true, label %bb_false

bb_true:
  call void @f(i32 %a, i32 %b, i32 %c)
  %x = add i32 %a, %b
  %y = mul i32 %b, %c
  %cond2 = icmp eq i32 %b, %y
  call void @f(i32 %a, i32 %b, i32 %c)
  br i1 %cond2, label %bb_true2, label %bb_false

bb_true2:
  call void @f(i32 %a, i32 %b, i32 %c)
  %cond3 = icmp eq i32 %x, %c
  br label %bb_exit

bb_false:
  br label %bb_exit2

bb_exit:
  call void @f(i32 %a, i32 %b, i32 %c)
  ret i32 %b

bb_exit2:
  ret i32 %b
}

declare void @f(i32, i32, i32)
