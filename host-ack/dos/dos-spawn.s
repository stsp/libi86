#
/*
 * Copyright (c) 2022 TK Chia
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

#ifdef __MSDOS__
	.define	__dos_spawn
__dos_spawn:
	mov	bx, sp
	push	ds
	push	es
	push	si
	push	di
	push	bp
	push	ds
	pop	es
	movb	al, 2(bx)
	mov	dx, 4(bx)
	mov	bx, 6(bx)
	xor	cx, cx
	movb	ah, 0x4b
	cmpb	al, 4
	jnz	.not_aexec
	movb	ah, 0x80
.not_aexec:
	push	cs
	mov	si, .cont
	push	si
	mov	si, 0x9a90
	push	si
	mov	si, 0x21cd
	push	si
	mov	si, sp
	push	ss
	push	si
	retf
.cont:
	pop	si
	cli
	pop	ss
	mov	sp, si
	sti
	pop	bp
	pop	di
	pop	si
	pop	es
	pop	ds
	jmp	.__libi86_ret_set_errno
#endif  /* __MSDOS__ */
