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

/* Internal implementation of _setvideomode (.) for SuperVGA video modes. */

#include "libi86/internal/call-cvt-far.h"
#include "libi86/internal/arch.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (libi86_setvideomode_svga.S.LIBI86)

	.global	__libi86_setvideomode_svga
__libi86_setvideomode_svga:
	ENTER_BX_ (2)
	pushw	%es
	pushw	%di
	subw	$0x100,	%sp
	movw	%ss,	%di
	movw	%di,	%es
	movw	%sp,	%di
	MOV_ARG0W_BX_ (%cx)
	pushw	%cx
	movw	$0x4f01, %ax		/* Get information for the requested */
	int	$0x10			/* video mode */
	popw	%bx
	cmpw	$0x004f, %ax		/* Bail out on failure */
	jnz	.error
	movw	$0x4f02, %ax		/* Set the video mode */
	int	$0x10
	cmpw	$0x004f, %ax		/* Check again for any error */
	jnz	.error
	xorw	%ax,	%ax		/* Find & return the text row count */
	movw	%ax,	%es
	movb	%es:0x0484, %al
	incw	%ax
.done:
	addw	$0x100,	%sp
	popw	%di
	popw	%es
	RET_ (2)
.error:
	xorw	%ax,	%ax
	jmp	.done
