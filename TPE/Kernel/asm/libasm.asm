GLOBAL cpuVendor, accessClock, readKey, sysCallWrite

section .text
	
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


