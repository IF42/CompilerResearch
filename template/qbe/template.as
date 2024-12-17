.text
.balign 16
.globl _start
_start:
	endbr64
	pushq %rbp
	movq %rsp, %rbp
	callq main
	leaq exit_val(%rip), %rdi
	callq ExitProcess
	leave
	ret
.type _start, @function
.size _start, .-_start
/* end function _start */

.text
.balign 16
main:
	endbr64
	movl $0, %eax
	ret
.type main, @function
.size main, .-main
/* end function main */

.section .note.GNU-stack,"",@progbits
