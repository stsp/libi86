/*
 * Copyright (c) 2019 TK Chia
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

#include "libi86/internal/call-cvt.h"

	.code16
	.att_syntax prefix

	TEXT_ (dos_getfileattr.S.LIBI86)
	.global	_dos_getfileattr
_dos_getfileattr:
	ENTER_BX_ (4)
#ifndef __IA16_CALLCVT_REGPARMCALL
	MOV_ARG0W_BX_ (%dx)
	MOV_ARG2W_BX_ (%bx)
#else
	xchgw	%ax,	%dx
	xchgw	%ax,	%bx
#endif
	movw	$0x4300, %ax
	int	$0x21
	jc	1f
	movw	%cx,	(%bx)
	xorw	%ax,	%ax
0:
	RET_ (4)
1:
	RET_SET_ERRNO_ (4, 0b)
