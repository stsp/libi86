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

	TEXT_ (libi86_call5.S.LIBI86)
#ifdef __MSDOS__
	.global	__libi86_call5
__libi86_call5:
	ENTER_BX_(6)
	pushw	%si
	pushw	%di
	pushw	%es
	pushw	%bp
	MOV_ARG0B_BX_(%cl)
	MOV_ARG2W_BX_(%dx)
	MOV_ARG4B_BX_(%al)
	/*
	 * PSP:0x0005 only handles a near return address, but we are most
	 * probably running in a code segment other than the PSP segment.
	 * Fortunately, on MS-DOS 2+ there is a
	 *	int	$0x21
	 *	lretw
	 * code sequence at offset PSP:0x0050, & we can get PSP:0x0005 to
	 * return to that...
	 */
	pushw	%cs
	movw	$1f,	%bx
	pushw	%bx
	movw	$0x0050+2, %bx
	pushw	%bx
	pushw	_psp
	movb	$0x0005, %bl
	pushw	%bx
	lretw
1:
	popw	%bp
	popw	%es
	popw	%di
	popw	%si
	pushw	%ss
	popw	%ds
	RET_(6)
#else
# error
#endif