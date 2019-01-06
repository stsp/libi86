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

/* Capture the contents of the current screen page.  This is a tiny memory
   model program.  */

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	.text
	.global	main
main:
	pushw	%si
	pushw	%di
	pushw	%bp
	xorw	%ax,	%ax		/* get number of rows on screen */
	movw	%ax,	%ds		/* (assumes EGA video or above!) */
	movb	0x484,	%cl
	pushw	%ss
	popw	%ds
	incw	%cx
	jcxz	.done
	mov	%cl,	rows
	movb	$0x0f,	%ah		/* get number of columns on screen, */
	int	$0x10			/* and active display page */
	movb	%bh,	page
	movb	%ah,	cols
	movb	$0x03,	%ah		/* store the current cursor position */
	int	$0x10
	movw	%dx,	curs_pos
	movw	$buf,	%di
	xorw	%dx,	%dx
.loop:					/* now loop through the whole screen */
	movb	$0x02,	%ah
	movb	page,	%bh
	pushw	%dx
	pushw	%di
	int	$0x10
	movb	$0x08,	%ah
	movb	page,	%bh
	int	$0x10
	popw	%di
	popw	%dx
	cmpb	$'\n',	%al		/* forbid stray '\n' characters */
	jz	.error
	cld
	stosb
	incb	%dl
	cmpb	cols,	%dl
	jnz	.loop
	movb	$'\n',	%al		/* add a '\n' after each row */
	stosb
	xorb	%dl,	%dl
	incb	%dh
	cmpb	rows,	%dh
	jnz	.loop
	movb	$0x40,	%ah		/* we are done; write the captured */
	movw	$1,	%bx		/* screen contents to stdout */
	movw	$buf,	%dx
	movw	%di,	%cx
	subw	%dx,	%cx
	int	$0x21
	jc	.error
	cmpw	%cx,	%ax
	jnz	.error
.done:
	xorw	%ax,	%ax
	popw	%bp
	popw	%di
	popw	%si
	ret
.error:
	movw	$1,	%ax
	popw	%bp
	popw	%di
	popw	%si
	ret

	.lcomm	page, 1
	.lcomm	cols, 1
	.lcomm	rows, 1
	.lcomm	curs_pos, 2
	.lcomm	buf, 0xc000
