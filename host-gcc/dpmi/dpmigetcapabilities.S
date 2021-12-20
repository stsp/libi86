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

	TEXT_ (dpmigetcapabilities.S.LIBI86)
#ifdef __MSDOS__
	.global	_DPMIGetCapabilities
_DPMIGetCapabilities:
	ENTER_BX_ (10)
	pushw	%di
	pushw	%es
	pushw	ARG4W_BX_
	pushw	ARG2W_BX_
	pushw	ARG0W_BX_
	LES_ARG6W_BX_ (%di)
	movw	$0x0401, %ax
	int	$0x31
	jc	.error
	popw	%bx
	movw	%ax,	(%bx)
	popw	%bx
	movw	%cx,	(%bx)
	popw	%bx
	movw	%dx,	(%bx)
.done:
	sbbw	%ax,	%ax
	popw	%es
	popw	%di
	RET_ (10)
.error:
	popw	%bx
	popw	%bx
	popw	%bx
	jmp	.done
#else
# warning "unknown target OS; _DPMIGetCapabilities (...) not implemented"
#endif
