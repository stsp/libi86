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

	TEXT_ (libi86_sound_by_divisor.S.LIBI86)
	.global	__libi86_sound_by_divisor
__libi86_sound_by_divisor:
	ENTER_BX_(2)
	MOV_ARG0W_BX_CLOBBER_(%dx)
	pushfw
	cli
	inb	$0x61,	%al
	orb	$0x03,	%al
	outb	%al,	$0x61
	movb	$0xb6,	%al
	outb	%al,	$0x43
	xchgw	%dx,	%ax
	outb	%al,	$0x42
	movb	%ah,	%al
	outb	%al,	$0x42
	popfw
	RET_(2)
