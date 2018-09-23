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

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"
#include "libi86/internal/struc.h"

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
	PUSH_IMM_VIA_(.done, %si)
	pushw	ARG0W_BX_		/* intr_call */
	MOV_ARG2W_BX_(%bx)		/* in_regs */
	LOAD_UNION_REGS_BX_
	ret
.done:
	pushw	%bx
	movw	%ss,	%bx		/* restore %ds */
	movw	%bx,	%ds
	movw	%sp,	%bx
	movw	2(%bx),	%bx		/* out_regs */
	STORE_UNION_REGS_BX_POP_CLOBBER_
	popw	%cx
	cld
	popw	%es
	popw	%di
	popw	%si
	popw	%bp
	RET_(6)
