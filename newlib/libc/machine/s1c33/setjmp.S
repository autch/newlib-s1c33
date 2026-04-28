/*
 * setjmp for the EPSON S1C33000 core (S5U1C33000C ABI).
 *
 * jmp_buf layout (matches _JBLEN=6, _JBTYPE=int in machine/setjmp.h):
 *   env[0..3] = callee-saved R0-R3
 *   env[4]    = saved SP (entry value: points at return-PC slot)
 *   env[5]    = saved return PC (loaded from [SP] at entry)
 *
 * S1C33 has no link register; `call` pushes the return PC onto the
 * stack and `ret` pops it (sp += 4).  setjmp therefore reads the
 * return PC from [SP] at entry.
 *
 * Args: %r12 = env
 * Return: %r10 = 0
 */

	.text
	.global	setjmp
	.type	setjmp, @function
setjmp:
	ld.w	[%r12]+, %r0	; env[0] = R0
	ld.w	[%r12]+, %r1	; env[1] = R1
	ld.w	[%r12]+, %r2	; env[2] = R2
	ld.w	[%r12]+, %r3	; env[3] = R3
	ld.w	%r4, %sp	; R4 = SP
	ld.w	[%r12]+, %r4	; env[4] = SP
	ld.w	%r5, [%r4]	; R5 = *(SP) = return PC
	ld.w	[%r12]+, %r5	; env[5] = return PC
	ld.w	%r10, 0		; return 0
	ret
	.size	setjmp, .-setjmp
