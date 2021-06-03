GLOBAL scan, print, dumpRegs, dumpMem, accessClock, screenClear, testAccessClock, UDcaller

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro



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

    pushState
	
	mov rsi, rdi ; buffer
	mov rdi, 2   ; interrupt id
	int 80h
	
	popState

    leave
	ret

dumpMem:
	pushState
	push rbp
	mov rbp, rsp
	
	mov rcx, rdx ; coloco en rcx la cant de bytes
	mov rdx, rsi ; rdx -> direc
	mov rsi, rdi ; seteo el buff
	mov rdi, 3
	int 80h 
	
	leave
	popState
	ret


testAccessClock:
	push rbp
	mov rbp, rsp

	push rdx
    push rsi
	push rdi
	
	mov rdx, rsi ; buff
	mov rsi, rdi ; seconds/minutes/hours/date/month/year id
	mov rdi, 6   ; interrupt id
	int 80h
	
	pop rdi
	pop rsi
	pop rdx

    leave
	ret


accessClock:
	push rbp
	mov rbp, rsp

	push rdx
    push rsi
	push rdi
	
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

	pushState

	mov rdi, 5
	int 80h

	popState
	leave
	ret

UDcaller:
	UD0 ; https://www.felixcloutier.com/x86/ud
	ret
