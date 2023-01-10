/*
 * Copyright (c) 2023 TK Chia
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

	.arch	i286, jumps
	.code16
	.att_syntax prefix

	TEXT_ (libi86_dpmi_32bit.S.LIBI86)

	/*
	 * Figure out whether we were started as a 32-bit DPMI application
	 * (even though we are running 16-bit code), which may happen if the
	 * program was built in "dual mode" (-mdosx32). If so, we need to
	 * clear the high halves of certain 32-bit registers when calling
	 * certain DPMI functions.
	 *
	 * FIXME: this is all incredibly ugly.
	 */
.Lctor_dpmi16_shim:
#ifndef __IA16_FEATURE_DOSX
	CALL_ (__DPMI_hosted)
	decw	%ax
	jnz	.done2
#endif
	movw	$0x0400, %ax		/* if the CPU is an 80286, then */
	int	$0x31			/* we are done */
	cmpb	$2,	%cl
	jz	.done2
	.arch	i386			/* not 286?  assume 386+ */
	movw	$0x0202, %ax		/* back up vector for exception 4 */
	movb	$4,	%bl		/* (overflow) */
	int	$0x31
	jc	.done2
	pushw	%cx			/* temporarily direct exception 4 */
	pushl	%edx			/* to ourselves */
	incw	%ax
	movw	%cs,	%cx
	movl	$.Lisr,	%edx
	int	$0x31
	jc	.done
	rorb	%al			/* %al was 3 */
	into				/* trigger overflow exception */
	into				/* trigger again; one of these
					   `into's will be 2-byte misaligned */
.done:					/* restore exception 4 vector */
	popl	%edx
	popw	%cx
	movw	$0x0203, %ax
	int	$0x31
.done2:
	RET_ (0)

.Lisr:
	testb	$1,	0xc(%esp)	/* 0xc(%esp) should be caller %sp
					   (aligned) for 16-bit frame, lower
					   half of caller %eip (possibly
					   unaligned) for 32-bit frame */
	jz	.maybe16bit
.32bit:
	movb	$1,	__libi86_dpmi_32bit
	movw	$.Ldpmi16_32, __libi86_dpmi16_shim
	movw	$.Ldpmimux16_32, __libi86_dpmimux16_shim
	lretl
.maybe16bit:
	cmpw	$0,	0xe(%esp)	/* 0xe(%esp) should be caller %ss
					   for 16-bit frame, upper half of
					   caller %eip for 32-bit frame */
	jz	.32bit
	lretw

.Ldpmi16_16:
	int	$0x31
	RET_ (0)

.Ldpmimux16_16:
	int	$0x2f
	RET_ (0)

.Ldpmi16_32:
	pushl	%edi
	popw	%di
	movzx	%di,	%edi
	int	$0x31
	pushw	%di
	popl	%edi
	RET_ (0)

.Ldpmimux16_32:
	pushl	%esi
	popw	%si
	movzx	%si,	%esi
	pushl	%edi
	popw	%di
	movzx	%di,	%edi
	int	$0x2f
	pushw	%di
	popl	%edi
	pushw	%si
	popl	%esi
	RET_ (0)

	.section .data, "aw"

	.global	__libi86_dpmi16_shim
__libi86_dpmi16_shim:
	TEXT_PTR_ (.Ldpmi16_16)
	.global	__libi86_dpmimux16_shim
__libi86_dpmimux16_shim:
	TEXT_PTR_ (.Ldpmimux16_16)

	.section .ctors.65435

	TEXT_PTR_ (.Lctor_dpmi16_shim)

	.comm	__libi86_dpmi_32bit, 1
