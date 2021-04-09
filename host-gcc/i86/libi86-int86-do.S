/*
 * Copyright (c) 2018--2019 TK Chia
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

/*
 * Internal implementation routine used by both the inline and out-of-line
 * versions of int86 (...) and _int86f (...).  This takes as its first
 * argument not an interrupt number, but a pointer to a routine that calls
 * the interrupt.
 */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"
#include "libi86/internal/struc.h"

	.code16
	.att_syntax prefix

	.text				/* N.B. */
#ifndef _BORLANDC_SOURCE
	.global	__libi86_int86_do
__libi86_int86_do:
#else
	.global	__libi86_bc_int86_do
__libi86_bc_int86_do:
#endif
	ENTER_BX_(6)
	pushw	%bp
	pushw	%si
	pushw	%di
	pushw	%es
	movw	%ds,	%si		/* good idea to set %es = %ds here */
	movw	%si,	%es
	pushw	ARG4W_BX_		/* out_regs */
	PUSH_IMM_VIA_(.done, %si)
	pushw	ARG0W_BX_		/* intr_call */
	MOV_ARG2W_BX_(%bx)		/* in_regs */
	LOAD_UNION_REGS_BX_
	ret
.done:
	pushw	%bx
	movw	%ss,	%bx		/* restore %ds */
	movw	%bx,	%ds
	movw	%sp,	%bx
	movw	2(%bx),	%bx		/* out_regs */
	STORE_UNION_REGS_BX_POP_CLOBBER_
	popw	%cx
	cld
	popw	%es
	popw	%di
	popw	%si
	popw	%bp
	RET_(6)
