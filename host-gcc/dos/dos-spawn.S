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
#include "libi86/internal/arch.h"

	.code16
	.att_syntax prefix

	TEXT_ (dos_spawn.S.LIBI86)
	.global	_dos_spawn
_dos_spawn:
	ENTER_BX_ (6)
	pushw	%ds
	pushw	%es
	pushw	%si
	pushw	%di
	pushw	%bp
	pushw	%ds
	popw	%es
	MOV_ARG0B_BX_ (%al)
	MOV_ARG2W_BX_ (%dx)
	MOV_ARG4W_BX_ (%bx)
	xorw	%cx,	%cx
	movb	$0x4b,	%ah
	cmpb	$4,	%al
	jnz	.not_aexec
	movb	$0x80,	%ah
.not_aexec:
#ifndef __IA16_FEATURE_DOSX
	/*
	 * If we are in real or V86 mode, then according to RBIL, for int
	 * 0x21, %ah = 0x4b, "DOS 2.x destroys all registers, including
	 * SS:SP".
	 *
	 * However, the syscall obviously needs to return to the correct
	 * %cs:%ip, i.e. it cannot destroy %cs:%ip.  So what we can do is
	 * to point %cs:%ip into our current stack, so that the return
	 * %cs:%ip gives us %ss:%sp.  And we can place code on the stack
	 * since we are not in protected mode. :-)
	 *
	 * Push the instruction sequence
	 *	int $0x21
	 *	nop
	 *	lcall $OUR_TEXT_SEG, $.cont
	 * onto the stack, & branch to it.
	 */
	pushw	%cs
	PUSH_IMM_VIA_ (.cont, %si)
	PUSH_IMM_VIA_ (0x9a90, %si)
	PUSH_IMM_VIA_ (0x21cd, %si)
	movw	%sp,	%si
	pushw	%ss
	pushw	%si
	lretw
.cont:
	popw	%si
	cli
	popw	%ss
	movw	%si,	%sp
	sti
#else  /* __IA16_FEATURE_DOSX */
	/*
	 * In DOS extender mode, %ss:%sp is preserved, & we need not worry
	 * about any of the above silliness. :-|
	 */
	int	$0x21
#endif	/* __IA16_FEATURE_DOSX */
	popw	%bp
	popw	%di
	popw	%si
	popw	%es
	popw	%ds
	RET_SET_ERRNO_ (6)
