GLOBAL cpuVendor, accessClock, readKey, sysCallWrite, infoReg, printMem

EXTERN printf

section .text

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


cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

accessClock:
	push rbp
	mov rbp, rsp

	mov al, dil
	out 70h, al
	xor rax, rax
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

;espera a leer del teclado y cuando devuelve algo te lo devuelve
readKey:
	push rbp
	mov rbp, rsp

.loop:
	in al, 64h
	and al, 00000001b
	jz .loop
	xor rax, rax
	in al, 60h

	leave
	ret

sysCallWrite:
	mov rax, 1
	int 80h
	ret


infoReg:
    push rbp
    mov rbp, rsp
    
	push rdi
	push rsi

    mov rdi, raxMsg
    mov rsi, rax
    call printf

	mov rdi, rbxMsg
    mov rsi, rbx
    call printf

	mov rdi, rcxMsg
    mov rsi, rcx
    call printf

	mov rdi, rdxMsg
    mov rsi, rdx
    call printf

	mov rdi, rsiMsg
    pop rsi
    call printf

	pop rdi
	mov rsi, rdi
	mov rdi, rdiMsg
    call printf

	mov rdi, rbpMsg
    mov rsi, rbp
    call printf

	mov rdi, rspMsg
    mov rsi, rsp
    call printf

	mov rdi, r8Msg
    mov rsi, r8
    call printf

	mov rdi, r9Msg
    mov rsi, r9
    call printf

	mov rdi, r10Msg
    mov rsi, r10
    call printf

	mov rdi, r11Msg
    mov rsi, r11
    call printf

	mov rdi, r12Msg
    mov rsi, r12
    call printf

	mov rdi, r13Msg
    mov rsi, r13
    call printf

	mov rdi, r14Msg
    mov rsi, r14
    call printf

	mov rdi, r15Msg
    mov rsi, r15
    call printf

    leave
    ret

;-------------------------------------------|
;   printMem: Recibe por parametro en RSI   |
;   la dirección donde debe comenzar a      |
;   dumpear.                                |
;-------------------------------------------|
printMem:
    pushState
    mov rcx, 0
    xor rdx, rdx


.loop:
    mov al, byte[rsi]
    inc rsi

    mov [buffer + rcx], al
    inc rcx

    cmp rcx, 32 ;muevo 32 bytes
    jne .loop

	mov byte[buffer + rcx], 0
	mov rdi, buffer
	call printf

    popState
    ret


section .data
	raxMsg db "RAX: 0x%x", 0
	rbxMsg db "RBX: 0x%x", 0
	rcxMsg db "RCX: 0x%x", 0
	rdxMsg db "RDX: 0x%x", 0
	rsiMsg db "RSI: 0x%x", 0
	rdiMsg db "RDI: 0x%x", 0
	rbpMsg db "RBP: 0x%x", 0
	rspMsg db "RSP: 0x%x", 0
	r8Msg db "R8: 0x%x", 0
	r9Msg db "R9: 0x%x", 0
	r10Msg db "R10: 0x%x", 0
	r11Msg db "R11: 0x%x", 0
	r12Msg db "R12: 0x%x", 0
	r13Msg db "R13: 0x%x", 0
	r14Msg db "R14: 0x%x", 0
	r15Msg db "R15: 0x%x", 0

	hexa db "0123456789ABCDEF", 0

section .bss
	buffer resb 35

