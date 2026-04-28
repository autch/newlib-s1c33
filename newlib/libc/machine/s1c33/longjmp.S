/*
 * longjmp for the EPSON S1C33000 core (S5U1C33000C ABI).
 *
 * Restores the register state captured by setjmp and returns control
 * to the address that setjmp would have returned to, with the return
 * value substituted with `val` (or 1 if val == 0).
 *
 * S1C33 has no link register; the saved return PC is reinstalled at
 * [SP] so the trailing `ret` pops it as the new PC and adjusts SP.
 *
 * Args: %r12 = env, %r13 = val
 * Return: jumps to env[5] with %r10 = val (or 1 if val == 0)
 */

	.text
	.global	longjmp
	.type	longjmp, @function
longjmp:
	ld.w	%r0, [%r12]+	; R0 = env[0]
	ld.w	%r1, [%r12]+	; R1 = env[1]
	ld.w	%r2, [%r12]+	; R2 = env[2]
	ld.w	%r3, [%r12]+	; R3 = env[3]
	ld.w	%r4, [%r12]+	; R4 = env[4] (saved SP)
	ld.w	%r5, [%r12]+	; R5 = env[5] (saved return PC)
	ld.w	%sp, %r4	; restore SP
	ld.w	[%r4], %r5	; *(SP) = return PC (so `ret` jumps there)
	cmp	%r13, 0
	jrne	.Llongjmp_ret
	ld.w	%r13, 1		; if val == 0, substitute 1
.Llongjmp_ret:
	ld.w	%r10, %r13
	ret
	.size	longjmp, .-longjmp
