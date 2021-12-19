/*
 * Copyright (c) 2021 TK Chia
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

	TEXT_ (fmemcmp.S.LIBI86)
	.global	_fmemcmp
_fmemcmp:
	ENTER2_BX_ (10)
	pushw	%ds
	pushw	%es
	pushw	%si
	pushw	%di
	MOV_ARG8W2_BX_ (%cx)
	LES_ARG4W2_BX_ (%di)
	LDS_ARG0W_BX_ (%si)
	shrw	%cx
	sbbw	%ax,	%ax
	jcxz	.done_words
	repz cmpsw
	jnz	.unequal_word
.done_words:
	xchgw	%ax,	%cx
	jcxz	.munge_flags
.final_byte:
	cmpsb
.munge_flags:
	lahf	/* SZ-A-P-C */
	rorb	%ah
	andw	$0b1010000000000000, %ax  /* CF set => ret. < 0 */
		/* CSZ-A-P- */		  /* CF clear, ZF set => ret. == 0 */
	xorw	$0b0010000000000000, %ax  /* CF, ZF clear => ret > 0 */
	popw	%di
	popw	%si
	popw	%es
	popw	%ds
	RET2_ (10)
.unequal_word:
	decw	%si
	decw	%si
	decw	%di
	decw	%di
	cmpsb
	jnz	.munge_flags
	jmp	.final_byte
