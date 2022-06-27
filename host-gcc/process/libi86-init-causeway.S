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

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/acconfig.h"

	.arch	i286, jumps
	.code16
	.att_syntax prefix

/*
 * In DOS extender mode, some libi86 modules only work properly with the
 * CauseWay (or compatible) DOS extender.  These modules link in the code
 * below to check if the DOS extender at runtime is indeed CauseWay.
 *
 * Such a check is probably prudent, as there is a practice of replacing a
 * program's DOS extender with a different DOS extender.  (For example,
 * DOS/32A describes itself as "a drop-in replacement for the DOS/4GW DOS
 * Extender and compatibles" (https://sourceforge.net/projects/dos32a/).)
 *
 * Try to make the check happen early during the initialization of
 * newlib-ia16's C runtime.
 */

#define epsp_parent	0x100

	.section .msdos_init.causeway, "ax"

	pushaw
	pushw	%es
	movw	$0xff00, %ax
	xorw	%bx,	%bx
	clc
	int	$0x31
	jnc	.cont
.oops:
	pushw	%ss
	popw	%ds
	movb	$9,	%ah
	movw	$.msg,	%dx
	int	$0x21
	movw	$0x4cff, %ax
	int	$0x21
.cont:
	lsl	%bx,	%ax		/* check if %bx is a valid selector */
	jnz	.oops
	cmpw	$epsp_parent+1, %ax
	jb	.oops
	movw	%bx,	%es		/* check if %bx:0 is an EPSP */
	cmpw	$0x20cd, %es:0
	jnz	.oops
	movw	%es:epsp_parent, %bx	/* check if parent field points to
					   a valid selector too... */
	lsl	%bx,	%ax
	jnz	.oops
	cmpw	$epsp_parent+1, %ax
	jb	.oops
	movw	%bx,	%es		/* ...& is also an EPSP */
	cmpw	$0x20cd, %es:0
	jnz	.oops
	popw	%es
	popaw

	.section .msdos_init_rodata, "a"

	.balign	2
.msg:	.ascii	"Not running on CauseWay-compatible DOS extender!$"
	.balign	2

	.global	__libi86_have_init_causeway
	.set	__libi86_have_init_causeway, 1
