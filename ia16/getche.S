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

#include "libi86/internal/call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	.text
#ifdef __MSDOS__
	.global	_getche
	.weak	getche
_getche:
getche:
	xorw	%cx,	%cx
	xchgw	%cx,	__libi86_ungetch_buf
	jcxz	.Lget
	xchgw	%ax,	%cx
	movb	$0,	%ah
	RET_(0)
.Lget:
	movb	$0x01,	%ah
	jmp	__libi86_conio_int21
#else
# warning "unknown target OS; getche () not implemented"
#endif
