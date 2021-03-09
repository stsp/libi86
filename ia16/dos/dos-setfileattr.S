/*
 * Copyright (c) 2019 TK Chia
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

	TEXT_ (dos_setfileattr.S.LIBI86)
	.global	_dos_setfileattr
_dos_setfileattr:
	ENTER_BX_ (4)
	MOV_ARG2W_BX_ (%cx)
	MOV_ARG0W_BX_CLOBBER_ (%dx)
	movw	$0x4301, %ax
	int	$0x21
	RET_SET_ERRNO_ (4)
