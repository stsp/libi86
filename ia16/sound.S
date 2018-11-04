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
	.global	__libi86_sound
	.global	sound
__libi86_sound:
sound:
	ENTER_BX_(2)
	MOV_ARG0W_BX_CLOBBER_(%cx)
	movw	$0x34dd, %ax		/* 1193181 == 0x1234dd */
	cwtd
	cmpw	$19,	%cx
	jb	.Lplay			/* %dx == 0 */
	movw	%cx,	%bx
	shrw	%bx
	addw	%bx,	%ax
	adcw	$0x12,	%dx
	divw	%cx			/* this will never overflow */
	xchgw	%ax,	%dx
.Lplay:
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
