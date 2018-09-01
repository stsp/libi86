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

#include "call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	.text
	.global	_fmemcpy
	.global	__libi86_fmemmove_forward
_fmemcpy:
__libi86_fmemmove_forward:
	ENTER2_BX_(10)
	pushw	%ds
	pushw	%es
	pushw	%si
	pushw	%di
	MOV_ARG8W2_BX_(%cx)
	MOV_ARG0W_BX_(%ax)		/* we will return DEST as %dx:%ax */
	MOV_ARG2W_BX_(%dx)
	movw	%ax,	%di
	movw	%dx,	%es
	LDS_ARG4W2_BX_(%si)
	shrw	$1,	%cx
	rep movsw
	adcw	%cx,	%cx
	rep movsb
	popw	%di
	popw	%si
	popw	%es
	popw	%ds
	RET2_(10)
