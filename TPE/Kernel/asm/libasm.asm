GLOBAL cpuVendor, accessClock, readKey, sysCallWrite, getRegister, dumpMem, asmGetByte, getDateTime

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

getDateTime:
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
	and al, 1
	jz .loop
	xor rax, rax
	in al, 60h

	leave
	ret

sysCallWrite:
	mov rax, 1
	int 80h
	ret


getRegister:
    push rbp
    mov rbp, rsp

	push rdi
    
.rax:	cmp rdi, 0
		jne .rbx
    	jp .exit

.rbx:	cmp rdi, 1
		jne .rcx
    	mov rax, rbx
    	jp .exit

.rcx:	cmp rdi, 2
		jne .rdx
    	mov rax, rcx
    	jp .exit

.rdx:	cmp rdi, 3
		jne .rsi
    	mov rax, rdx
    	jp .exit	

.rsi:	cmp rdi, 4
		jne .rdi
    	mov rax, rsi
    	jp .exit

.rdi:	cmp rdi, 5
		jne .rbp
		pop rdi
    	mov rax, rdi
    	jp .exit

.rbp:	cmp rdi, 6
		jne .rsp
    	mov rax, rbp
    	jp .exit

.rsp:	cmp rdi, 7
		jne .r8
    	mov rax, rsp
    	jp .exit

.r8:	cmp rdi, 8
		jne .r9
    	mov rax, r8
    	jp .exit

.r9:	cmp rdi, 9
		jne .r10
    	mov rax, r9
    	jp .exit

.r10:	cmp rdi, 10
		jne .r11
    	mov rax, r10
    	jp .exit

.r11:	cmp rdi, 11
		jne .r12
    	mov rax, r11
    	jp .exit

.r12:	cmp rdi, 12
		jne .r13
    	mov rax, r12
    	jp .exit

.r13:	cmp rdi, 13
		jne .r14
    	mov rax, r13
    	jp .exit

.r14:	cmp rdi, 14
		jne .r15
    	mov rax, r14
    	jp .exit

.r15:	cmp rdi, 15
		jne .exit
    	mov rax, r15
    	jp .exit

.exit 	
    	mov rsp, rbp
		pop rbp
    	ret


;----------------
;asmGetByte:
;Recibe en rdi la dirección de donde levantar el dato
;-----------------

asmGetByte:
    pushState
    push rbp
    mov rbp, rsp
    
    mov al, byte[rdi]
    
    leave
    popState
    ret

section .data
	hexa db "0123456789ABCDEF", 0

section .bss
	buffer resb 35
