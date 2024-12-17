global _start

section .text
_start:
	push rbp
	mov rbp, rsp
	sub rsp, 0

	mov rsp, rbp
	pop rbp
	mov rcx, 0
	ret

main:
	push rbp
	mov rbp, rsp
	sub rsp, 16

	mov rax, 2
	mov [rbp-8], rax
	neg qword [rbp-8]

	not qword [rbp-16]

	mov rsp, rbp
	pop rbp
	mov rcx, [rbp-16]
	ret

