/*
 * Copyright (c) 2020 TK Chia
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

	TEXT_ (bios_joystick.S.LIBI86)
	.global	_bios_joystick
_bios_joystick:
	pushw	%di
#ifndef __IA16_CALLCVT_REGPARMCALL
	movw	%sp,	%di
# ifdef __IA16_CMODEL_IS_FAR_TEXT
	movw	6(%di), %dx
	movw	8(%di),	%di
# else
	movw	4(%di), %dx
	movw	6(%di),	%di
# endif
	movb	$0x84,	%ah
	int	$0x15
	jc	.error
#else
	movw	%dx,	%di
	xchgw	%ax,	%dx
	movb	$0x84,	%ah
	int	$0x15
	jc	.error
#endif
	movw	%ax,	(%di)
	movw	%bx,	2(%di)
	movw	%cx,	4(%di)
	movw	%dx,	6(%di)
	xorw	%ax,	%ax
.error:
	pop	%di
	RET_ (4)
