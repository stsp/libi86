/*
 * Copyright (c) 2018--2019 TK Chia
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
 * versions of intr (...) and _intrf (...).  This takes as its first argument
 * not an interrupt number, but a pointer to a routine that calls the
 * interrupt.
 */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"
#include "libi86/internal/struc.h"

	.code16
	.att_syntax prefix

	.text				/* N.B. */
	.global	__libi86_intr_do
__libi86_intr_do:
	ENTER_BX_(4)
	pushw	%bp
	pushw	%si
	pushw	%di
	pushw	%es
	MOV_ARG2W_BX_(%di)		/* regs */
	pushw	%di
	PUSH_IMM_VIA_(.done, %si)
	pushw	ARG0W_BX_		/* intr_call */
	LOAD_UNION_REGPACK_DI_
	ret
.done:
	pushw	%bp
	movw	%sp,	%bp
	movw	2(%bp),	%bp		/* regs */
	STORE_UNION_REGPACK_BP_POP_
	popw	%ax
	cld
	movw	%ss,	%ax
	movw	%ax,	%ds
	popw	%es
	popw	%di
	popw	%si
	popw	%bp
	RET_(4)
