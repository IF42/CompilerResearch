extern _start
section .text
_start:
	call main
	mov rdi, rax
	mov rax, 60
	syscall

main:
	push rsp
	mov rbp, rsp
	mov rax, 10
	cdq
	mov r10, 3
	idiv r10
	mov qword [rbp - 8], rdx
	mov rax, qword [rbp - 8]
	mov rsp, rbp
	pop rbp
	ret
