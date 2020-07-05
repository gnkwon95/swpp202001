define i32 @count_even(i32* %arr, i32 %N) {
	entry:
		%cnt = alloca i32
		%loopcount = alloca i32
		store i32 0, i32* %cnt
		store i32 0, i32* %loopcount
		br label %loop
	
	loop:
		%0 = load i32, i32* %loopcount
		%cmp = icmp ult i32 %0, %N
		br i1 %cmp, label %loop.else, label %loop.end

	loop.else:
		%p = load i32, i32* %loopcount
		%arrayidx = getelementptr inbounds i32, i32* %arr, i32 %p
		%1 = load i32, i32* %arrayidx
		
		%2 = load i32, i32* %loopcount
		%3 = add i32 %2, 1
		store i32 %3, i32* %loopcount
		
		%cmp2 = srem i32 %1, 2
		%cmp3 = icmp eq i32 %cmp2, 0
		br i1 %cmp3, label %ifDivisible, label %loop

	ifDivisible:
		%4 = load i32, i32* %cnt
		%5 = add i32 %4, 1
		store i32 %5, i32* %cnt
		br label %loop

	loop.end:
		%6 = load i32, i32* %cnt
		ret i32 %6
}

