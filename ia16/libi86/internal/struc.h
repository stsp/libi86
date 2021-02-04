/*
 * Macros to load and store various structure types in assembly language code.
 *
 * Copyright (c) 2018 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING2.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
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

/* Load registers and the SZAPC flags from a `union REGPACK' at (%di).  Note
   that %ds might no longer point to the program's data segment after this.  */
#define LOAD_UNION_REGPACK_DI_		\
	movb	18(%di), %ah;		\
	sahf;				\
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
