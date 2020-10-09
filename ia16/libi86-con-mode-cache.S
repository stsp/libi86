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
 * Cache of the current video mode & number of rows of text displayed on
 * screen, & routines to update the cache.
 */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"
#include "graph.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (libi86_con_mode_cache.S.LIBI86)

.error:					/* From below */
	movb	$0x0f,	%ah		/* If VESA fails, use the classical */
	int	$0x10			/* interface to get the video mode */
	andw	$0x007f, %ax
	jmp	__libi86_con_mode_changed

.Lctor_con_mode_cache:
	movw	$0x4f03, %ax		/* Try to retrieve the current */
	int	$0x10			/* mode via VESA */
	cmpw	$0x004f, %ax
	jz	.error
	xchgw	%ax,	%bx
	andb	$0x7f,	%ah
	/* fall through */

	/* On entry, %ax == new mode. */
	.global	__libi86_con_mode_changed
__libi86_con_mode_changed:		/* on entry, %ax == new mode */
	movw	%ax,	__libi86_con_mode_cache
	pushw	%es			/* Get the current screen width & */
	movw	__libi86_bios_ds, %es	/* height */
	movw	%es:0x004a, %ax
	movw	%ax,	__libi86_con_mode_cache+2
	movb	%es:0x0084, %al
	popw	%es
	andw	$0x00ff, %ax
	jnz	.got_rows
	movb	$24,	%al
.got_rows:
	incw	%ax
	movw	%ax,	__libi86_con_mode_cache+4
	JMP_ (__libi86_con_reset_wnd)	/* Reset the text window */

	/*
	 * Note: this constructor must run before __libi86_setvideomode_
	 * default () is primed.
	 */
	.section .ctors.65436

	.balign	2
	TEXT_PTR_ (.Lctor_con_mode_cache)

	/* This must agree with "libi86/internal/conio.h"! */
	.comm	__libi86_con_mode_cache, 4
