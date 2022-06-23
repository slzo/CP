	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movl	-12(%rbp), %eax
	cltq
	pushq	%rax
	call	__isoc99_scanf@PLT
	addq	$4,%rsp
	movl	-12(%rbp), %eax
	pushq	%rax
	call	__isoc99_scanf@PLT
	addq	$4,%rsp
	movl	-12(%rbp), %eax
	pushq	%rax
	movq	-16(%rbp), %rax
	popq	%rbx
	addq	%rbx, %rax
	movq	%rax, -20(%rbp)
	movq	-20(%rbp), %rax
	pushq	%rax
	call	printf@PLT
	addq	$4,%rsp
	movq	$0, %rax
	jmp		.LRET_main
.LRET_main:
	addq	$16, %rsp
	popq	%rbp
	ret
