GLOBAL scan, print, dumpRegs, dumpMem, accessClock, clearScreen

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

    pushState

    mov rcx ,rdx ;len
    mov rdx, rsi  ;buffer
    mov rsi,rdi   ; fd
    mov rdi,0    ; interrupt id
    int 80h
    
    popState

    leave
    ret
    
print:
    push rbp
    mov rbp,rsp;

    pushState

    mov rcx ,rdx ;len
    mov rdx, rsi  ;buffer
    mov rsi,rdi   ; fd
    mov rdi,1     ; interrupt id
    int 80h

    popState

    leave
    ret


dumpRegs:
	push rbp
	mov rbp, rsp

    pushState
	
	mov rsi, rdi ; buffer
	mov rax, 2   ; interrupt id
	int 80h
	
	popState

    leave
	ret

dumpMem:
	push rbp
	mov rbp, rsp

    pushState
	
	mov rcx, rdx ; len
	mov rdx, rsi ; address
	mov rsi, rdi ; buffer
	mov rax, 3   ; interrupt id
	int 80h
	
	popState

    leave
	ret

accessClock:
	push rbp
	mov rbp, rsp

    pushState
	
	mov rsi, rdi ; seconds/minutes/hours/date/month/year id
	mov rax, 4   ; interrupt id
	int 80h
	
	popState

    leave
	ret

clearScreen:
	push rbp
	mov rbp, rsp

	pushState

	mov rdi, 5 ;interrupt id
	int 80h

	popState
	leave
	ret