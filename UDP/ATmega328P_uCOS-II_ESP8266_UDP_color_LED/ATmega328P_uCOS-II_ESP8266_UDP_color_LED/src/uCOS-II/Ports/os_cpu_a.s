	.text
	.align

	.extern  OSRunning
	.extern  OSPrioCur
	.extern  OSPrioHighRdy
	.extern  OSTCBCur
	.extern  OSTCBHighRdy
;	.extern  OSTaskSwHook

	.global CPUGetSR
	.global CPUSetSR
	.global OSStartHighRdy
	.global OSCtxSw

	.equ SREG, 0x3F
	.equ SP_H, 0x3E
	.equ SP_L, 0x3D

CPUGetSR:
	in r24, SREG
	cli
	ret

CPUSetSR:
	out SREG, r24
	ret

OSStartHighRdy:
;	call OSTaskSwHook
	ldi r24, 0x01
	sts OSRunning, r24
	lds r26, OSTCBHighRdy
	lds r27, OSTCBHighRdy + 1
	ld r28, X+
	ld r29, X+
	out SP_L, r28
	out SP_H, r29
	pop r0
	out SREG, r0
	pop r31
	pop r30
	pop r29
	pop r28
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	ret

OSCtxSw:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r28
	push r29
	push r30
	push r31
	in r0, SREG
	push r0
	in r28, SP_L
	in r29, SP_H
	lds r26, OSTCBCur
	lds r27, OSTCBCur + 1
	st X+, r28
	st X+, r29
;	call OSTaskSwHook
	lds r24, OSPrioHighRdy
	sts OSPrioCur, r24
	lds r26, OSTCBHighRdy
	lds r27, OSTCBHighRdy + 1
	sts OSTCBCur, r26
	sts OSTCBCur + 1, r27
	ld r28, X+
	ld r29, X+
	out SP_L, r28
	out SP_H, r29
	pop r0
	out SREG, r0
	pop r31
	pop r30
	pop r29
	pop r28
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	ret

	.end