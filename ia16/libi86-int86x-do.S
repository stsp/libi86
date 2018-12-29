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
 * versions of int86 (...) and _int86f (...).  This takes as its first
 * argument not an interrupt number, but a pointer to a routine that calls
 * the interrupt.
 */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"
#include "libi86/internal/struc.h"

	.code16
	.att_syntax prefix

	.text
	.global	__libi86_int86x_do
__libi86_int86x_do:
	ENTER_BX_(8)
	pushw	%bp
	pushw	%si
	pushw	%di
	pushw	%es
	MOV_ARG6W_BX_(%si)		/* seg_regs */
	pushw	%si
	movw	(%si),	%es
	pushw	ARG4W_BX_		/* out_regs */
	PUSH_IMM_VIA_(.done, %di)
	pushw	ARG0W_BX_		/* intr_call */
	pushw	6(%si)
	MOV_ARG2W_BX_(%bx)		/* in_regs */
	LOAD_UNION_REGS_BX_
	popw	%ds
	ret
.done:
	pushw	%bx
	movw	%sp,	%bx
	movw	%ss:4(%bx), %bx		/* seg_regs */
	movw	%ds,	%ss:6(%bx)	/* store %ds from interrupt call */
	pushw	%ss			/* restore %ds */
	popw	%ds
	movw	%es,	(%bx)		/* store %es, %cs, %ss */
	movw	%cs,	2(%bx)
	movw	%ss,	4(%bx)
	movw	%sp,	%bx
	movw	2(%bx),	%bx		/* out_regs */
	STORE_UNION_REGS_BX_POP_CLOBBER_
	popw	%cx
	popw	%cx
	cld
	popw	%es
	popw	%di
	popw	%si
	popw	%bp
	RET_(8)
