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

#include "libi86/internal/call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (sound.S.LIBI86)
	.global	__libi86_sound
	.global	sound
__libi86_sound:
sound:
	ENTER_BX_(2)
	MOV_ARG0W_BX_CLOBBER_(%cx)
	movw	$0x34dd, %ax		/* 1193181 == 0x1234dd */
	cwtd
	cmpw	$19,	%cx
	jb	.Lplay			/* %dx == 0 */
	movw	%cx,	%bx
	shrw	%bx
	addw	%bx,	%ax
	adcw	$0x12,	%dx
	divw	%cx			/* this will never overflow */
	xchgw	%ax,	%dx
.Lplay:
	pushfw
	cli
	inb	$0x61,	%al
	orb	$0x03,	%al
	outb	%al,	$0x61
	movb	$0xb6,	%al
	outb	%al,	$0x43
	xchgw	%dx,	%ax
	outb	%al,	$0x42
	movb	%ah,	%al
	outb	%al,	$0x42
	popfw
	RET_(2)
