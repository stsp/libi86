/*
 * Copyright (c) 2018 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING3.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
 */

/*
 * Internal implementation routine used by both the inline and out-of-line
 * versions of int86 (...).  This takes as its first argument not an
 * interrupt number, but a pointer to a routine that calls the interrupt.
 */

#include "call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	.text
	.global	__libi86_int86_do
__libi86_int86_do:
	ENTER_BX_(6)
	pushw	%bp
	pushw	%si
	pushw	%di
	pushw	%es
	movw	%ds,	%si		/* good idea to set %ds = %es here */
	movw	%si,	%es
	MOV_ARG4W_BX_(%si)		/* out_regs */
	pushw	%si
	movw	$.done,	%si
	pushw	%si
	pushw	ARG0W_BX_		/* intr_call */
	MOV_ARG2W_BX_(%bx)		/* in_regs */
	cmpw	$1,	14(%bx)		/* .. load the carry flag */
	cmc				/* .:			  */
	movw	(%bx),	%ax
	movw	4(%bx),	%cx
	movw	6(%bx),	%dx
	movw	8(%bx),	%si
	movw	10(%bx), %di
	movw	12(%bx), %bp
	mov	2(%bx),	%bx
	ret
.done:
	pushw	%bx
	movw	%ss,	%bx		/* restore %ds */
	movw	%bx,	%ds
	movw	%sp,	%bx
	movw	2(%bx),	%bx		/* out_regs */
	movw	%ax,	(%bx)
	popw	%ax
	movw	%ax,	2(%bx)
	popw	%ax
	movw	%cx,	4(%bx)
	movw	%dx,	6(%bx)
	movw	%si,	8(%bx)
	movw	%di,	10(%bx)
	movw	%bp,	12(%bx)
	sbbw	%ax,	%ax
	movw	%ax,	14(%bx)
	popw	%es
	popw	%di
	popw	%si
	popw	%bp
	movw	(%bx),	%ax
	RET_(6)
