global _start

section .text
_start:
	mov rax, 0x3C
	xor rdi, rdi
	syscall
