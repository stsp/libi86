/*
 * Copyright (c) 2018--2020 TK Chia
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the developer(s) nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
	cld
	cmpb	$' ',	%al		/* escape special characters */
	jb	.special
	cmpb	$'~',	%al
	jae	.special
.special_ok:
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
.special:				/* encode special characters as `~' */
	xchgw	%ax,	%bx		/* followed by an octal char. code */
	movb	$'~',	%al
	stosb
	movb	%bl,	%bh
	rolb	%bl
	rolb	%bl
	movb	%bl,	%al
	andb	$0x03,	%al
	orb	$'0',	%al
	stosb
	rolb	%bl
	rolb	%bl
	rolb	%bl
	movb	%bl,	%al
	andb	$0x07,	%al
	orb	$'0',	%al
	stosb
	movb	%bh,	%al
	andb	$0x07,	%al
	orb	$'0',	%al
	jmp	.special_ok

	.lcomm	page, 1
	.lcomm	cols, 1
	.lcomm	rows, 1
	.lcomm	curs_pos, 2
	.lcomm	buf, 0xc000
