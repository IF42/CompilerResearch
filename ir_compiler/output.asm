extern printf
extern ExitProcess
global _start
global main

section .data
	fmt db "%d", 10, 0

section .text

_start:
	call main

main:
	push 35
	push 34

	pop rax
	pop rbx
	sub rax, rbx
	push rax

	pop rax
	mov rcx, fmt
	mov rdx, rax
	call printf

	push 42
	push 1

	pop rax
	pop rbx
	add rax, rbx
	push rax

	pop rax
	mov rcx, fmt
	mov rdx, rax
	call printf

	mov rcx, rax
	call ExitProcess
