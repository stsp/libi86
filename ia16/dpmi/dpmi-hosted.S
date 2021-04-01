/*
 * Copyright (c) 2020 TK Chia
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
#include "libi86/internal/acconfig.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (dpmi_hosted.S.LIBI86)
#ifdef __MSDOS__
# ifndef _LIBI86_INTERNAL_HAVE___DPMI_HOSTED
	.weak	__DPMI_hosted
__DPMI_hosted:
# endif
	.global	__libi86_DPMI_hosted
__libi86_DPMI_hosted:
	/*
	 * We could try using int 0x2f, ax = 0x1686, but the _DOS Protected
	 * Mode Interface (DPMI)_ version 1.0 warns,
	 *
	 *	"This function should not be used to determine if a DPMI host
	 *	 is present. A client should make sure that DPMI services are
	 *	 available before calling this function; otherwise, the
	 *	 results returned by the function may not be valid."
	 *
	 * So instead, we do some tests with the segment registers to see if
	 * they behave like in real mode (or V86 mode) or like in protected
	 * mode.
	 */
	subw	$0x30,	%sp
	movw	%sp,	%bx
	movw	$1,	%ax
	movb	%ah,	(%bx)
	movb	%al,	0x10(%bx)
	movb	$2,	0x20(%bx)
	/*
	 * Frob the lowest bit of the segment value.
	 *   * In real or V86 mode, the resulting value will refer to the
	 *     segment one paragraph higher or lower than our data segment.
	 *   * In protected mode, this will refer to the same segment, with
	 *     only a different requested privilege level (RPL).
	 */
	movw	%ds,	%cx
	xorb	%al,	%cl
	movw	%cx,	%ds
	/* Return 1 if running under DPMI, -1 otherwise. */
	cmpb	%al,	0x10(%bx)
	jz	.pm
	negw	%ax
.pm:
	movw	%ss,	%cx
	movw	%cx,	%ds
	addw	$0x30,	%sp
	RET_ (0)
#else
# warning "unknown target OS; __DPMI_hosted () not implemented"
#endif
