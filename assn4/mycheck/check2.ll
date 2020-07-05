define i32 @main(i32 %b, i32 %a) {


; CHECK-LABEL: @main(i32 %b, i32 %a)
; CHECK-NEXT: [[COND:%.*]] = icmp eq i32 [[A:%.*]], [[B:%.*]]
; CHECK-NEXT: br i1 [[COND]], label [[BB_TRUE:%.*]], label [[BB_FALSE:%.*]]



; CHECK: bb_exit:
; CHECK-NEXT: call void @f(i32 [[B]], i32 [[A]])
; CHECK-NEXT: ret i32 [[A]]

; CHECK: bb_true:
; CHECK-NEXT: [[C:%.*]] = add i32 [[B]], 0
; CHECK-NEXT: [[X:%.*]] = add i32 [[B]], [[B]]
; CHECK-NEXT: [[COND2:%.*]] = icmp eq i32 [[X]], [[B]]
; CHECK-NEXT: call void @f(i32 [[B]], i32 [[X]])
; CHECK-NEXT: br i1 [[COND2]], label [[BB_TRUE2:%.*]], label [[BB_FALSE]]

; CHECK: bb_true2:
; CHECK-NEXT: call void @f(i32 [[B]], i32 [[B]])
; CHECK-NEXT: [[COND3:%.*]] = icmp eq i32 [[B]], [[C]]
; CHECK-NEXT: br i1 [[COND3]], label [[BB_TRUE]], label [[BB_EXIT:%.*]]

; CHECK: bb_false:
; CHECK-NEXT: br label [[BB_EXIT]]
;
  %cond = icmp eq i32 %a, %b
  br i1 %cond, label %bb_true, label %bb_false

bb_exit:
  call void @f(i32 %b, i32 %a) 
  ret i32 %a

bb_true:
  %c = add i32 %a, 0
  %x = add i32 %a, %b
  %cond2 = icmp eq i32 %x, %b
  call void @f(i32 %a, i32 %x)
  br i1 %cond2, label %bb_true2, label %bb_false

bb_true2:
  call void @f(i32 %a, i32 %b)
  %cond3 = icmp eq i32 %x, %c
  br i1 %cond3, label %bb_true, label %bb_exit

bb_false:
  br label %bb_exit
}

declare void @f(i32, i32)
