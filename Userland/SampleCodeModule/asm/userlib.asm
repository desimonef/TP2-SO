GLOBAL scan, print, dumpRegs, dumpMem, accessClock, screenClear, UDcaller

scan:
    push rbp
    mov rbp,rsp;

    push rdi
    push rsi
    push rdx
    push rcx

    mov rcx ,rdx ;len
    mov rdx, rsi  ;buffer
    mov rsi,rdi   ; fd
    mov rdi,0    ; interrupt id
    int 80h
    
    pop rcx
    pop rdx
    pop rsi
    pop rdi

    leave
    ret
    
print:
    push rbp
    mov rbp,rsp;

    push rdi
    push rsi
    push rdx
    push rcx

    mov rcx ,rdx ;len
    mov rdx, rsi  ;buffer
    mov rsi,rdi   ; fd
    mov rdi,1     ; interrupt id
    int 80h

    pop rcx
    pop rdx
    pop rsi
    pop rdi

    leave
    ret

dumpRegs:
	push rbp
	mov rbp, rsp

	push rdi
    	push rsi
	
	mov rsi, rdi ; buffer
	mov rdi, 2   ; interrupt id
	int 80h
	
	pop rsi
    	pop rdi

    leave
	ret

dumpMem:

	push rbp
	mov rbp, rsp

	push rdi
    push rsi
    push rdx
    push rcx
	
	mov rcx, rdx ; coloco en rcx la cant de bytes
	mov rdx, rsi ; rdx -> direc
	mov rsi, rdi ; seteo el buff
	mov rdi, 3
	int 80h 
	
	pop rcx
    pop rdx
    pop rsi
    pop rdi

	leave
	ret


accessClock:
	push rbp
	mov rbp, rsp

	push rdx
    push rsi
	push rdi
	
	mov rdx, rsi ; buff
	mov rsi, rdi ; seconds/minutes/hours/date/month/year id
	mov rdi, 4   ; interrupt id
	int 80h
	
	pop rdi
	pop rsi
	pop rdx

    leave
	ret


screenClear:
	push rbp
	mov rbp, rsp

	push rsi
	push rdi

	mov rsi, rdi
	mov rdi, 5
	int 80h

	pop rdi
	pop rsi
	leave
	ret

UDcaller:
	UD0 ; https://www.felixcloutier.com/x86/ud
	ret
