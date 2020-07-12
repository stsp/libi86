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
 * Internal implementation of _setvideomode (.) for the "classical" MDPA &
 * CGA video modes, namely _TEXT{BW, C}{40, 80}, _MRES{4, NO}COLOR, _HRESBW,
 * & _TEXTMONO.
 */

#include "libi86/internal/call-cvt-far.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (setvideomode_mdpa_cga.S.LIBI86)

	.global	__libi86_setvideomode_mdpa_cga
__libi86_setvideomode_mdpa_cga:
	ENTER_BX_ (2)
	MOV_ARG0W_BX_ (%ax)
	pushw	%ax
	int	$0x10
	movb	$0x0f,	%ah
	int	$0x10
	popw	%dx
	cmp	%dl,	%al
	movb	$0,	%al
	jnz	.error
	movb	$25,	%al
.error:
	cbtw
	RET_ (2)
