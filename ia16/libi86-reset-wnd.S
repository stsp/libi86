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
 * Routine to reset the text window after changing (or obtaining) the video
 * mode.  On input, %cx is non-zero if the video mode is known to be a
 * classical mode; otherwise, %cx == 0.
 *
 * This file also holds the variable to keeps track of the current text
 * window.
 */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (libi86_reset_wnd.S.LIBI86)

.Lctor_reset_wnd:
	.global	__libi86_reset_wnd_svga
__libi86_reset_wnd_svga:
	xorw	%cx,	%cx
	/* fall through */

	.global	__libi86_reset_wnd
__libi86_reset_wnd:
	pushw	%es
	movw	$1,	%ax		/* Always reset x1 & y1 first */
	movw	%ax,	__libi86_con_wnd
	movw	%ax,	__libi86_con_wnd+2
	jcxz	.try_svga		/* If we might be dealing with a
					   SuperVGA mode, try using VESA to
					   get the mode information first */
.fallback:				/* Otherwise --- or if VESA fails */
	movw	__libi86_bios_ds, %es	/* --- get the information from */
	movw	%es:0x004a, %bx		/* the IBM BIOS data area */
	movb	%es:0x0084, %al
	andw	$0x00ff, %ax
	jnz	.got_rows
	movb	$24,	%al
.got_rows:
	incw	%ax
.done:					/* Update the window information, &
					   return the text row count */
	movw	%bx,	__libi86_con_wnd+4
	movw	%bx,	__libi86_con_wnd+8
	movw	%ax,	__libi86_con_wnd+6
	movw	%ax,	__libi86_con_wnd+10
	pop	%es
	RET_ (0)
.fallback2:
	addw	$0x100, %sp
	popw	%di
	jmp	.fallback
.try_svga:				/* We might be dealing with a
					   SuperVGA mode */
	movw	$0x4f03, %ax		/* Use VESA to get the current */
	int	$0x10			/* video mode */
	cmpw	$0x004f, %ax
	jnz	.fallback
	testw	$~0x807f, %bx		/* If not an SVGA mode, fall back */
	jz	.fallback		/* on the old method */
	pushw	%di			/* Otherwise, get video mode info. */
	pushw	%ss			/* via VESA */
	popw	%es
	subw	$0x100, %sp
	movw	$0x4f01, %ax
	movw	%bx,	%cx
	movw	%sp,	%di
	int	$0x10
	cmpw	$0x004f, %ax
	jnz	.fallback2
	testb	$0x02,	(%di)		/* If the "optional" mode info. */
	jz	.fallback2		/* (including width & height) are
					   not available, fall back on the
					   old method to get them */
	testb	$0x10,	(%di)
	jnz	.graph
	movw	0x12(%di), %bx		/* If this is a text mode, then */
	movw	0x14(%di), %ax		/* 0x12(%di) & 0x14(%di) give the
					   text width & text height
					   respectively */
.done_svga:
	addw	$0x100,	%sp		/* We can now simply return the */
	popw	%di			/* text row count :-) */
	jmp	.done
.graph:
	movb	0x16(%di), %cl		/* If this is a graphics mode, we */
	xorb	%ch,	%ch		/* need to do some division */
	jcxz	.fallback2		/* ?!?!? */
	cwtd				/* (%ax == 0x004f) */
	movw	0x12(%di), %ax
	divw	%cx
	xchgw	%ax,	%bx
	movb	0x17(%di), %cl
	jcxz	.fallback2
	movw	0x14(%di), %ax
	xorw	%dx,	%dx
	divw	%cx
	jmp	.done_svga

/* Note: this constructor must run only after __libi86_bios_ds is set. */
	.section .ctors.65434

	.balign	2
	TEXT_PTR_ (.Lctor_reset_wnd)

/* This must agree with "libi86/internal/conio.h"! */
	.comm	__libi86_con_wnd, 12
