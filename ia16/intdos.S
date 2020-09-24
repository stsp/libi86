/*
 * Copyright (c) 2018--2019 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING2.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
 */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"
#include "libi86/internal/struc.h"

	.code16
	.att_syntax prefix

	TEXT_ (intdos.S.LIBI86)
	.global	intdos
intdos:
	ENTER_BX_(4)
	pushw	%bp
	pushw	%si
	pushw	%di
	pushw	%es
	movw	%ds,	%si		/* good idea to set %es = %ds here */
	movw	%si,	%es
	pushw	ARG2W_BX_		/* out_regs */
	MOV_ARG0W_BX_(%bx)		/* in_regs */
	LOAD_UNION_REGS_BX_
	int	$0x21
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
	RET_(4)
