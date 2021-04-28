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

/* Internal implementation routine used by int86 (...) and _int86f (...). */

#include "libi86/internal/sect.h"
#include "libi86/internal/struc.h"

#ifndef _BORLANDC_SOURCE
	.define	___libi86_int86x_do
___libi86_int86x_do:
#else
	.define	___libi86_bc_int86x_do
___libi86_bc_int86x_do:
#endif
	mov	bx, sp
	push	bp
	push	si
	push	di
	push	es
	mov	si, 8(bx)		/* seg_regs */
	push	si
	mov	es, (si)
	push	6(bx)			/* out_regs */
	mov	di, .done
	push	di
	push	2(bx)			/* intr_call */
	push	6(si)
	mov	bx, 4(bx)		/* in_regs */
	LOAD_UNION_REGS_BX_
	pop	ds
	ret
.done:
	push	bx
	mov	bx, sp
	sseg mov bx, 4(bx)		/* seg_regs */
	sseg mov 6(bx), ds		/* store ds from interrupt call */
	push	ss			/* restore ds */
	pop	ds
	mov	(bx), es		/* store es, cs, ss */
	mov	2(bx), cs
	mov	4(bx), ss
	mov	bx, sp
	mov	bx, 2(bx)		/* out_regs */
	STORE_UNION_REGS_BX_POP_CLOBBER_
	pop	cx
	pop	cx
	cld
	pop	es
	pop	di
	pop	si
	pop	bp
	ret
