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

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (lz4cpy.S.LIBI86)
	.global	_lz4cpy
	.weak	lz4cpy
_lz4cpy:
lz4cpy:
	ENTER2_BX_ (10)
	pushw	%si
	pushw	%di
	pushw	%es
	LES_ARG0W_BX_ (%di)		/* %es:%di := DEST */
	LDS_ARG4W2_BX_ (%si)		/* %ds:%si := BLK_SRC */
	ss MOV_ARG8W2_BX_ (%bx)		/* %bx := BLK_SZ */
	addw	%si,	%bx		/* %bx := FP_OFF (BLK_SRC + BLK_SZ) */
	jmp	.chk
.loopy:
	lodsb
	xchgw	%ax,	%dx
	mov	%dl,	%ch
	mov	$12,	%cl
	shrw	%cl,	%cx
	call	.full_len
	rep movsb
	cmpw	%bx,	%si
	jnb	.done
	lodsw
	movb	%dl,	%cl
	movw	%di,	%dx
	subw	%ax,	%dx
	andw	$0x0f,	%cx
	call	.full_len
	addw	$4,	%cx
	pushw	%ds
	pushw	%es
	popw	%ds
	xchgw	%dx,	%si
	rep movsb
	movw	%dx,	%si
	popw	%ds
.chk:
	cmpw	%bx,	%si
	jb	.loopy
.done:
	xchgw	%di,	%ax
	movw	%es,	%dx
	pushw	%ss
	popw	%ds
	popw	%es
	popw	%di
	popw	%si
	RET2_ (10)

.full_len:
	cmpb	$0x0f,	%cl
	jnz	9f
	movb	$0,	%ah
1:
	lodsb
	addw	%ax,	%cx
	cmp	$0xff,	%al
	jz	1b
9:
	ret
