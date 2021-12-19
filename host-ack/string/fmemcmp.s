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

	.define	__fmemcmp
__fmemcmp:
	mov	bx, sp
	push	ds
	push	es
	push	si
	push	di
	mov	cx, 10(bx)		/* n */
	les	di, 6(bx)		/* s2 */
	lds	si, 2(bx)		/* s1 */
	shr	cx, 1
	sbb	ax, ax
	jcxz	.done_words
	repz cmpsw
	jnz	.unequal_word
.done_words:
	xchg	cx, ax
	jcxz	.munge_flags
.final_byte:
	cmpsb
.munge_flags:
	lahf	   /* SZ-A-P-C */
	rorb	ah, 1
	and	ax, 0b1010000000000000	/* CF set => ret. < 0 */
		   /* CSZ-A-P- */	/* CF clear, ZF set => ret. == 0 */
	xor	ax, 0b0010000000000000	/* CF, ZF clear => ret > 0 */
	pop	di
	pop	si
	pop	es
	pop	ds
	ret
.unequal_word:
	dec	si
	dec	si
	dec	di
	dec	di
	cmpsb
	jnz	.munge_flags
	jmp	.final_byte
