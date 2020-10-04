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

/*
 * Internal implementation of _setvideomode (.) for the "classical" MDPA,
 * CGA, EGA, & VGA video modes.
 */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (libi86_setvideomode_nonsvga.S.LIBI86)

	.global	__libi86_setvideomode_nonsvga
__libi86_setvideomode_nonsvga:
	ENTER_BX_ (2)
	MOV_ARG0W_BX_ (%ax)
	pushw	%ax
	int	$0x10			/* Set the video mode; %ah == 0 */
	movw	$0x4f03, %ax		/* Try to retrieve the new video */
	int	$0x10			/* mode via VESA, & check it */
	cmpw	$0x004f, %ax
	jnz	.retry
	popw	%dx
	cmpw	%bx,	%dx
	jnz	.error			/* If we successfully set the */
.ok:					/* video mode, reset the text window */
	movb	$1,	%cl		/* & return the text row count, else */
	CALL_ (__libi86_reset_wnd)	/* say 0 */
	RET_ (2)
.retry:					/* If using VESA fails, try to */
	movb	$0x0f,	%ah		/* retrieve the new video mode via */
	int	$0x10			/* the traditional interface */
	popw	%dx
	cmp	%dl,	%al
	jz	.ok
.error:
	xorw	%ax,	%ax
	RET_ (2)
