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
 * versions of intr (...).  This takes as its first argument not an
 * interrupt number, but a pointer to a routine that calls the interrupt.
 */

#include "call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	.text
	.global	__libi86_intr_do
__libi86_intr_do:
	ENTER_BX_(4)
	pushw	%bp
	pushw	%si
	pushw	%di
	pushw	%es
	MOV_ARG2W_BX_(%di)		/* regs */
	pushw	%di
	movw	$.done,	%si
	pushw	%si
	pushw	ARG0W_BX_		/* intr_call */
	movb	18(%di), %ah		/* load only the SZAPC flags */
	sahf
	movw	(%di),	%ax
	movw	2(%di),	%bx
	movw	4(%di),	%cx
	movw	6(%di),	%dx
	movw	8(%di),	%bp
	movw	10(%di), %si
	movw	16(%di), %es
	ldsw	12(%di), %di
	ret
.done:
	pushw	%bp
	movw	%sp,	%bp
	movw	2(%bp),	%bp		/* regs */
	movw	%ax,	(%bp)
	popw	%ax
	movw	%ax,	8(%bp)
	popw	%ax
	movw	%bx,	2(%bp)
	movw	%cx,	4(%bp)
	movw	%dx,	6(%bp)
	movw	%si,	10(%bp)
	movw	%di,	12(%bp)
	movw	%ds,	14(%bp)
	movw	%es,	16(%bp)
	pushfw
	popw	%ax
	movw	%ax,	18(%bp)
	movw	%ss,	%ax
	movw	%ax,	%ds
	popw	%es
	popw	%di
	popw	%si
	popw	%bp
	RET_(4)
