#
/*
 * Copyright (c) 2018--2021 TK Chia
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

	.sect	.text
	.sect	.rom
	.sect	.data
	.sect	.bss

	.sect	.text

	xor	cx, cx			/* get number of rows on screen */
	mov	ds, cx			/* (assumes EGA video or above!) */
	movb	cl, (0x484)
	push	ss
	pop	ds
	inc	cx
	jcxz	.done
	movb	(rows), cl
	movb	ah, 0x0f		/* get number of columns on screen, */
	int	0x10			/* and active display page */
	movb	(page), bh
	movb	(cols), ah
	mov	di, buf
	xor	dx, dx
.loop:					/* now loop through the whole screen */
	movb	ah, 0x02
	movb	bh, (page)
	push	dx
	push	di
	int	0x10
	movb	ah, 0x08
	movb	bh, (page)
	int	0x10
	pop	di
	pop	dx
	cld
	cmpb	al, ' '			/* escape special characters */
	jb	.special
	cmpb	al, '~'
	jae	.special
.special_ok:
	stosb
	incb	dl
	cmpb	dl, (cols)
	jnz	.loop
	movb	al, '\n'		/* add a '\n' after each row */
	stosb
	xorb	dl, dl
	incb	dh
	cmpb	dh, (rows)
	jnz	.loop
	movb	ah, 0x40		/* we are done; write the captured */
	mov	bx, 1			/* screen contents to stdout */
	mov	dx, buf
	mov	cx, di
	sub	cx, dx
	int	0x21
	jc	.error
	cmp	ax, cx
	jnz	.error
.done:
	mov	ax, 0x4c00
	int	0x21
.error:
	mov	ax, 0x4c01
	int	0x21
.special:				/* encode special characters as `~' */
	xchg	bx, ax			/* followed by an octal char. code */
	movb	al, '~'
	stosb
	movb	bh, bl
	rolb	bl, 1
	rolb	bl, 1
	movb	al, bl
	andb	al, 0x03
	orb	al, '0'
	stosb
	rolb	bl, 1
	rolb	bl, 1
	rolb	bl, 1
	movb	al, bl
	andb	al, 0x07
	orb	al, '0'
	stosb
	movb	al, bh
	andb	al, 0x07
	orb	al, '0'
	jmp	.special_ok

	.sect	.bss

	.comm	page, 1
	.comm	cols, 1
	.comm	rows, 1
	.comm	curs_pos, 2
	.comm	buf, 0xc000
