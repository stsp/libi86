/*
 * Copyright (c) 2020 TK Chia
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

	.code16
	.att_syntax prefix

	TEXT_ (dos_allocmem.S.LIBI86)
	.global	_dos_allocmem
_dos_allocmem:
	ENTER_BX_ (4)
	pushw	%si
	MOV_ARG2W_BX_ (%si)
	MOV_ARG0W_BX_CLOBBER_ (%bx)
	movb	$0x48,	%ah
	int	$0x21
	jc	0f
	xchgw	%ax,	%bx
0:
	movw	%bx,	(%si)
	popw	%si
	RET_SET_ERRNO_ (4)
