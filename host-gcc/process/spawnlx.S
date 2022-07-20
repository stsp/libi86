/*
 * Copyright (c) 2022 TK Chia
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

#include "libi86/internal/arch.h"
#include "libi86/internal/call-cvt.h"
#include "libi86/internal/acconfig.h"

	.code16
	.att_syntax prefix

	TEXT_ (dos_spawnlx.S.LIBI86)

#if defined __TINY__ || defined __SMALL__
# define FAR_ADJ	0
#elif defined __MEDIUM__
# define FAR_ADJ	2
#else
# error "unknown memory model"
#endif

#if defined L_spawnl
	.global	_spawnl
	.weak	spawnl
_spawnl:
spawnl:
#elif defined L_spawnle
	.global	_spawnle
	.weak	spawnle
_spawnle:
spawnle:
#elif defined L_spawnlp
	.global	_spawnlp
	.weak	spawnlp
_spawnlp:
spawnlp:
#elif defined L_spawnlpe
	.global	_spawnlpe
	.weak	spawnlpe
_spawnlpe:
spawnlpe:
#else
# error "which function are we assembling?"
#endif
#if defined __IA16_CALLCVT_REGPARMCALL
	/*
	 * In a _spawnl... (MODE, PATH, ARG0 [, ARG1, ... NULL ...]) call,
	 * ARG0 is the last fixed argument, & _not_ part of the variable
	 * argument list.  (This follows newlib-ia16 &, well, pretty much
	 * everyone else.)
	 *
	 * For the `regparmcall' parameter passing convention, this means
	 * that ARG0 is still in a register at this point.  Try to fit
	 * ARG0 onto the stack such that we end up with a nicely laid out
	 * ARGV[] := { ARG0, ARG1, ARG2, ... NULL } array.  To do that, we
	 * need to move the function return address out of the way.
	 *
	 * We also later need to push the ENVP pointer before calling
	 * _spawnve (...) or _spawnvpe (...) call.  Unfortunately this call
	 * cannot be a tail call: the callee will pop ENVP, but neither the
	 * caller nor the callee will be responsible for popping the stack
	 * copy of ARG0.
	 */
	pushw	%ds			/* `pushw %ds' is a dummy push */
	pushw	%es
	pushw	%si
	pushw	%di
	pushw	%ax
	pushw	%ds
	popw	%es
	movw	%sp,	%di
	addw	$8,	%di
	leaw	2(%di),	%si
	movsw				/* move return %ip */
# ifdef __MEDIUM__
	movsw				/* move return %cs */
# endif
	movw	%di,	%ax		/* set %cx := ARGV; store ARG0 */
	xchgw	%ax,	%cx
	stosw
# if defined L_spawnle || defined L_spawnlpe
	/* Fish out ENVP by scanning for the null pointer after the ARG's. */
	.byte	0x3c			/* `cmpb ..., %al': skip 1 byte */
.find_envp:
	lodsw
	testw	%ax,	%ax
	jnz	.find_envp
	movw	(%si),	%bx		/* set %bx := ENVP */
# endif  /* spawnle || spawnlpe */
	popw	%ax
	popw	%di
	popw	%si
	popw	%es
# if defined L_spawnle || defined L_spawnlpe
	pushw	%bx			/* push ENVP */
# else
	PUSH_IMM_VIA_ (0, %bx)
# endif
# if defined L_spawnlp || defined L_spawnlpe
	CALL_ (_spawnvpe)
# else
	CALL_ (_spawnve)
# endif
# ifdef __MEDIUM__
	lretw	$2
# else
	retw	$2
# endif
#elif defined __IA16_CALLCVT_STDCALL  /* ! regparmcall */
	/*
	 * For the `stdcall' convention, ARG0 is already on the stack, _&_
	 * also we can do a tail call.  We still need to move the return
	 * address, & possibly locate ENVP, though.
	 */
	subw	$8,	%sp
	push	%es
	pushw	%si
	pushw	%di
	pushw	%ds
	popw	%es
	movw	%sp,	%di
	addw	$6,	%di
	leaw	8(%di),	%si
	movsw				/* move return %ip */
# ifdef __MEDIUM__
	movsw				/* move return %cs */
# endif
	movsw				/* move MODE */
	movsw				/* move PATH */
	movw	%si,	%ax		/* store ARGV */
	stosw
# if defined L_spawnle || defined L_spawnlpe
.find_envp:				/* look for ENVP */
	lodsw
	testw	%ax,	%ax
	jnz	.find_envp
	movsw				/* move ENVP */
# else  /* spawnl || spawnlp */
	xorw	%ax,	%ax		/* store ENVP = 0 */
	stosw
# endif  /* spawnl || spawnlp */
	pop	%di
	pop	%si
	pop	%es
# if defined L_spawnlp || defined L_spawnlpe
	JMP_ (_spawnvpe)
# else
	JMP_ (_spawnve)
# endif
#elif defined __IA16_CALLCVT_CDECL  /* ! regparmcall && ! stdcall */
	/* For `cdecl': no tail call.  :-| */
	subw	$8,	%sp
	push	%es
	pushw	%si
	pushw	%di
	pushw	%ds
	popw	%es
	movw	%sp,	%di
	addw	$6,	%di
	leaw	8+2+FAR_ADJ(%di), %si
	movsw				/* move MODE */
	movsw				/* move PATH */
	movw	%si,	%ax		/* store ARGV */
	stosw
# if defined L_spawnle || defined L_spawnlpe
.find_envp:				/* look for ENVP */
	lodsw
	testw	%ax,	%ax
	jnz	.find_envp
	movsw				/* move ENVP */
# else  /* spawnl || spawnlp */
	xorw	%ax,	%ax		/* store ENVP = 0 */
	stosw
# endif  /* spawnl || spawnlp */
	pop	%di
	pop	%si
	pop	%es
# if defined L_spawnlp || defined L_spawnlpe
	CALL_ (_spawnvpe)
# else
	CALL_ (_spawnve)
# endif
	addw	$8,	%sp
	RET_VA_
#else  /* ! regparmcall && ! stdcall && ! cdecl */
# error "unknown calling convention"
#endif  /* ! regparmcall && ! stdcall && ! cdecl */
