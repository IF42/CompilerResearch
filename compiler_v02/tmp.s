section .text
extern ExitProcess
extern _start
_start:
	call main
	mov rcx, rax
	call ExitProcess

main:
	push rsp
	mov rbp, rsp
	mov rax, 1
	mov rsp, rbp
	pop rbp
	ret
