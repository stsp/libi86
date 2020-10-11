/*
 * Copyright (c) 2020 TK Chia
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

/* Internal implementation of _setvideomode (.) for SuperVGA video modes. */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (libi86_setvideomode_svga.S.LIBI86)

	.global	__libi86_setvideomode_svga
__libi86_setvideomode_svga:
	ENTER_BX_ (2)
	MOV_ARG0W_BX_ (%bx)
	pushw	%bx
	movw	$0x4f02, %ax		/* Set the video mode */
	int	$0x10
	cmpw	$0x004f, %ax		/* Check for any error */
	popw	%ax
	jnz	.error
					/* Reset the text window */
	TAIL_CALL_ (__libi86_con_mode_changed, 2)
.error:
	xorw	%ax,	%ax
	RET_ (2)
