
#define _esp 0
#define _eip 4
/* ------ */
#define _rsp 0
#define _rip 8

.globl cuoco_ctx_swap

#ifndef __MACH__
.type  cuoco_ctx_swap, @function
#endif

cuoco_ctx_swap:

#if defined(__i386__)

	pushl %ebx
	pushl %ebp
	leal 8(%esp),%esp

	/* sp,ip */
	/* param old ctx */
	movl 4(%esp), %eax

	movl (%esp), %ecx
	movl %ecx, _eip(%eax)
	movl %esp, _esp(%eax)

	/* param new ctx */
	movl 8(%esp), %eax

	/* push ret ( ip,sp ) */
	movl _eip(%eax), %ecx
	movl _esp(%eax), %esp
	movl %ecx, (%esp)

	leal -8(%esp),%esp
	popl %ebp
	popl %ebx

	movl $0, %eax
	ret

#elif defined(__x86_64__)

	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx
	pushq %rbp
	leaq 48(%rsp),%rsp

	movq (%rsp), %rcx
	movq %rcx, _rip(%rdi)
	movq %rsp, _rsp(%rdi)

	movq _rsp(%rsi), %rsp
	movq _rip(%rsi), %rcx
	movq %rcx, (%rsp)

	leaq -48(%rsp),%rsp
	popq %rbp
	popq %rbx
	popq %r15
	popq %r14
	popq %r13
	popq %r12

	xorl %eax, %eax
	ret
#endif
