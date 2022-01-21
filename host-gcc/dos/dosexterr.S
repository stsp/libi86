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

	.code16
	.att_syntax prefix

	TEXT_ (dosexterr.S.LIBI86)
	.global	dosexterr
dosexterr:
	pushw	%bp
#ifdef __IA16_CALLCVT_REGPARMCALL
	pushw	%ax
#endif
	pushw	%si
	pushw	%di
	pushw	%ds
	pushw	%es
	movb	$0x59,	%ah
	xorw	%bx,	%bx
	int	$0x21
	popw	%es
	popw	%ds
	popw	%di
	popw	%si
#ifdef __IA16_CALLCVT_REGPARMCALL
	popw	%bp
#else
	movw	%sp,	%bp
# ifdef __MEDIUM__
	movw	6(%bp),	%bp
# else
	movw	4(%bp),	%bp
# endif
#endif
	movw	%ax,	(%bp)		/* .exterror */
	xchgb	%bl,	%bh
	movw	%bx,	2(%bp)		/* .errclass, .action */
	movb	%ch,	4(%bp)		/* .locus */
	popw	%bp
	RET_ (2)
