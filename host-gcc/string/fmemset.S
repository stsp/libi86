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

#include "libi86/internal/call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

#ifdef __IA16_CMODEL_IS_FAR_TEXT	/* 1st stack arg. offset from %di */
# define SA0		6
#else
# define SA0		4
#endif

	TEXT_ (fmemset.S.LIBI86)
	.global	_fmemset
_fmemset:
	pushw	%di
	movw	%sp,	%di
	pushw	%es
#ifdef __IA16_CALLCVT_REGPARMCALL	/* we will return DEST as %dx:%ax; */
	xchgw	%ax,	%bx		/* stash offset part in %bx first */
	xchgw	%ax,	%cx
	movw	SA0(%di), %cx
	movw	%dx,	%es
	movw	%bx,	%di
#else
	movb	SA0+4(%di), %al
	movw	SA0+6(%di), %cx
	lesw	SA0(%di), %di
	movw	%es,	%dx
	movw	%di,	%bx
#endif
	movb	%al,	%ah
	shrw	%cx
	rep stosw
	adcw	%cx,	%cx
	rep stosb
	xchgw	%ax,	%bx
	popw	%es
	popw	%di
	RET_ (8)
