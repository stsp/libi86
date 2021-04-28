#
/*
 * Copyright (c) 2021 TK Chia
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

	.define	___libi86_intr_dispatch
___libi86_intr_dispatch:
	mov	bx, sp
	movb	al, 2(bx)
	xorb	ah, ah
	mov	bx, ax
	shl	bx, 1
	add	ax, bx
	add	ax, .dispatch
	ret

#define INTS4(i) \
	int	[i];		\
	ret;			\
	int	[i] + 1;	\
	ret;			\
	int	[i] + 2;	\
	ret;			\
	int	[i] + 3;	\
	ret
#define INTS8(i)		\
	INTS4 (i);		\
	INTS4 ([i] + 4)
#define INTS16(i)		\
	INTS8 (i);		\
	INTS8 ([i] + 8)

.dispatch:
	int	0x00
	ret
	int	0x01
	ret
	int	0x02
	ret
	int	0x03			/* `int 3' is a 1-byte instruction; */
	ret				/* hence the `nop' */
	nop
	INTS4 (0x04)
	INTS8 (0x08)
	INTS16 (0x10)
	INTS4 (0x20)
#ifdef __MSDOS__
	int	0x24
	ret
	jmp	.int25
	jmp	.int26
	int	0x27
	ret
#else
	INTS4 (0x24)
#endif
	INTS8 (0x28)
	INTS16 (0x30)
	INTS16 (0x40)
	INTS16 (0x50)
	INTS16 (0x60)
	INTS16 (0x70)
	INTS16 (0x80)
	INTS16 (0x90)
	INTS16 (0xa0)
	INTS16 (0xb0)
	INTS16 (0xc0)
	INTS16 (0xd0)
	INTS16 (0xe0)
	INTS16 (0xf0)

#ifdef __MSDOS__
.int25:
	int	0x25
	jc	.cy
	popf
	clc
	ret
.int26:
	int	0x26
	jc	.cy
	popf
	clc
	ret
.cy:
	popf
	stc
	ret
#endif
