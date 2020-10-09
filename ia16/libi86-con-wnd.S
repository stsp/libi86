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
 * Variable to keep track of the current text window, & a routine to reset
 * the text window after changing (or obtaining) the video mode.
 */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (libi86_con_wnd.S.LIBI86)

	.global	__libi86_con_reset_wnd
__libi86_con_reset_wnd:
	movw	$1,	%ax		/* Reset x1 & y1 first */
	movw	%ax,	__libi86_con_wnd
	movw	%ax,	__libi86_con_wnd+2
					/* Now reset x2 & y2 */
	movw	__libi86_con_mode_cache+2, %ax
	movw	%ax,	__libi86_con_wnd+4
	movw	__libi86_con_mode_cache+4, %ax
	movw	%ax,	__libi86_con_wnd+6
	RET_ (0)			/* We are done; the text row count
					   is returned in %ax :-) */

/* This must agree with "libi86/internal/conio.h"! */
	.comm	__libi86_con_wnd, 8
