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

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (fstrlen.S.LIBI86)
	.global	_fstrlen
_fstrlen:
	ENTER_BX_(4)
	pushw   %di
	pushw   %es
	LES_ARG0W_BX_(%di)
	movw	%di,	%bx
	xorb	%al,	%al
	movw    $-1,    %cx
	repne   scasb
	movw    %di,    %ax
	subw	%bx,	%ax
	decw    %ax
	popw    %es
	popw    %di
	RET_(4)
