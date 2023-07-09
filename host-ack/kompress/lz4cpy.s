#
/*
 * Copyright (c) 2023 TK Chia
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

#include "libi86/internal/sect.h"

	.define	__lz4cpy
__lz4cpy:
	mov	bx, sp
	push	si
	push	di
	push	bp
	push	es
	les	di, 4(bx)		/* es:di := DEST */
	mov	bp, 12(bx)		/* bp := BLK_SZ */
	lds	si, 8(bx)		/* si := FP_OFF (BLK_SRC) */
	mov	bx, ds			/* bx := FP_SEG (BLK_SRC) */
	add	bp, si			/* bp := FP_OFF (BLK_SRC + BLK_SZ) */
	jmp	.chk
.loopy:
	lodsb
	xchg	dx, ax
	movb	ch, dl
	movb	cl, 12
	shr	cx, cl
	call	.full_len
	rep movsb
	cmp	si, bp
	jnb	.done
	lodsw
	movb	cl, dl
	mov	dx, di
	sub	dx, ax
	and	cx, 0x0f
	call	.full_len
	add	cx, 4
	push	es
	pop	ds
	xchg	si, dx
	rep movsb
	mov	si, dx
	mov	ds, bx
.chk:
	cmp	si, bp
	jb	.loopy
.done:
	push	ss
	pop	ds
	mov	bx, sp
	mov	bx, 8+2(bx)		/* return value := updated DEST */
	mov	(bx), di
	mov	2(bx), es
	xchg	bx, ax
	pop	es
	pop	bp
	pop	di
	pop	si
	ret

.full_len:
	cmpb	cl, 0x0f
	jnz	.9
	movb	ah, 0
.1:
	lodsb
	add	cx, ax
	cmpb	al, 0xff
	jz	.1
.9:
	ret
