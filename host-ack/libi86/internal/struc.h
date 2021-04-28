/*
 * Macros to load and store various structure types in assembly language code.
 *
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

#ifndef _LIBI86_INTERNAL_STRUC_H_
#define _LIBI86_INTERNAL_STRUC_H_

/* Load registers and the carry flag from a `union REGS' at (bx).  */
#define LOAD_UNION_REGS_BX_		\
	cmp	14(bx), 1;		\
	cmc;				\
	mov	ax, (bx);		\
	mov	cx, 4(bx);		\
	mov	dx, 6(bx);		\
	mov	si, 8(bx);		\
	mov	di, 10(bx);		\
	mov	bp, 12(bx);		\
	mov	bx, 2(bx)

/* Store registers and the carry flag to a `union REGS' at (bx), with the
   value of .w.bx popped from the stack.  Clobber cx.

   In Borland C compatibility mode, also store all the low order flags, not
   just the carry flag. */
#ifndef _BORLANDC_SOURCE
# define STORE_UNION_REGS_BX_POP_CLOBBER_ \
	mov	(bx), ax;		\
	pop	2(bx);			\
	mov	4(bx), cx;		\
	mov	6(bx), dx;		\
	mov	8(bx), si;		\
	mov	10(bx), di;		\
	mov	12(bx), bp;		\
	sbb	cx, cx;			\
	mov	14(bx), cx
#else
# define STORE_UNION_REGS_BX_POP_CLOBBER_ \
	mov	(bx), ax;		\
	pop	2(bx);			\
	mov	cx, 4(bx);		\
	mov	6(bx), dx;		\
	mov	8(bx), si;		\
	mov	10(bx), di;		\
	mov	12(bx), bp;		\
	pushf;				\
	pop	16(bx);			\
	sbb	cx, cx;			\
	mov	14(bx), cx
#endif

/* Load registers from a `union REGPACK' at (di) --- but not the flags,
   which are unchanged.  Note that ds might no longer point to the program's
   data segment after this.  */
#define LOAD_UNION_REGPACK_DI_		\
	mov	ax, (di);		\
	mov	bx, 2(di);		\
	mov	cx, 4(di);		\
	mov	dx, 6(di);		\
	mov	bp, 8(di);		\
	mov	si, 10(di);		\
	mov	es, 16(di);		\
	lds	di, 12(di)

/* Store registers and flags to a `union REGPACK' at ss:(bp), with the value
   of .w.bp popped from the stack.  */
#define STORE_UNION_REGPACK_BP_POP_	\
	mov	(bp), ax;		\
	mov	2(bp), bx;		\
	mov	4(bp), cx;		\
	mov	6(bp), dx;		\
	pop	8(bp);			\
	mov	10(bp), si;		\
	mov	12(bp), di;		\
	mov	14(bp), ds;		\
	mov	16(bp), es;		\
	pushf;				\
	pop	18(bp)

#endif
