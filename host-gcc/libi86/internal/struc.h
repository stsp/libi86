/*
 * Macros to load and store various structure types in assembly language code.
 *
 * Copyright (c) 2018 TK Chia
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

#ifndef __ASSEMBLER__
# error "not using this file in assembly code!"
#endif

/* Load registers and the carry flag from a `union REGS' at (%bx).  */
#define LOAD_UNION_REGS_BX_		\
	cmpw	$1,	14(%bx);	\
	cmc;				\
	movw	(%bx),	%ax;		\
	movw	4(%bx),	%cx;		\
	movw	6(%bx),	%dx;		\
	movw	8(%bx),	%si;		\
	movw	10(%bx), %di;		\
	movw	12(%bx), %bp;		\
	movw	2(%bx), %bx

/* Store registers and the carry flag to a `union REGS' at (%bx), with the
   value of .w.bx popped from the stack.  Clobber %cx.

   In Borland C compatibility mode, also store all the low order flags, not
   just the carry flag. */
#ifndef _BORLANDC_SOURCE
# define STORE_UNION_REGS_BX_POP_CLOBBER_ \
	movw	%ax,	(%bx);		\
	popw	2(%bx);			\
	movw	%cx,	4(%bx);		\
	movw	%dx,	6(%bx);		\
	movw	%si,	8(%bx);		\
	movw	%di,	10(%bx);	\
	movw	%bp,	12(%bx);	\
	sbbw	%cx,	%cx;		\
	movw	%cx,	14(%bx)
#else
# define STORE_UNION_REGS_BX_POP_CLOBBER_ \
	movw	%ax,	(%bx);		\
	popw	2(%bx);			\
	movw	%cx,	4(%bx);		\
	movw	%dx,	6(%bx);		\
	movw	%si,	8(%bx);		\
	movw	%di,	10(%bx);	\
	movw	%bp,	12(%bx);	\
	pushfw;				\
	popw	16(%bx);		\
	sbbw	%cx,	%cx;		\
	movw	%cx,	14(%bx)
#endif

/* Load registers from a `union REGPACK' at (%di) --- but not the flags,
   which are unchanged.  Note that %ds might no longer point to the
   program's data segment after this.  */
#define LOAD_UNION_REGPACK_DI_		\
	movw	(%di),	%ax;		\
	movw	2(%di),	%bx;		\
	movw	4(%di), %cx;		\
	movw	6(%di), %dx;		\
	movw	8(%di), %bp;		\
	movw	10(%di), %si;		\
	movw	16(%di), %es;		\
	ldsw	12(%di), %di

/* Store registers and flags to a `union REGPACK' at %ss:(%bp), with the value
   of .w.bp popped from the stack.  */
#define STORE_UNION_REGPACK_BP_POP_	\
	movw	%ax,	(%bp);		\
	movw	%bx,	2(%bp);		\
	movw	%cx,	4(%bp);		\
	movw	%dx,	6(%bp);		\
	popw	8(%bp);			\
	movw	%si,	10(%bp);	\
	movw	%di,	12(%bp);	\
	movw	%ds,	14(%bp);	\
	movw	%es,	16(%bp);	\
	pushfw;				\
	popw	18(%bp)

#endif
