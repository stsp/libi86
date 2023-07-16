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

	.define	__lz4len
__lz4len:
	mov	bx, sp
	push	si
	push	di
	lds	si, 2(bx)		/* ds:si := BLK_SRC */
	sseg mov bx, 6(bx)		/* bx := BLK_SZ */
	add	bx, si			/* bx := FP_OFF (BLK_SRC + BLK_SZ) */
	xor	di, di			/* di := UNPACKLEN */
	jmp	.chk
.loopy:
	lodsb
	xchg	dx, ax
	movb	ch, dl
	movb	cl, 12
	shr	cx, cl
	call	.full_len_ckd
	jc	.ouch
	add	si, cx
	add	di, cx
	jc	.ouch
	cmp	si, bx
	jnb	.done
	lodsw
	movb	cl, dl
	and	cx, 0x0f
	call	.full_len_ckd
	jc	.ouch
	add	cx, 4
	jc	.ouch
	add	di, cx
	jc	.ouch
.chk:
	cmp	si, bx
	jb	.loopy
.done:
	xchg	di, ax
	.data1	0x3d			/* cmp ax, ... */
.ouch:
	xor	ax, ax
	push	ss
	pop	ds
	pop	di
	pop	si
	ret

.full_len_ckd:
	cmpb	cl, 0x0f
	jnz	.8
	movb	ah, 0
.1:
	lodsb
	add	cx, ax
	jc	.9
	cmpb	al, 0xff
	jz	.1
.8:
	clc
.9:
	ret
