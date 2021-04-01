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

/*
 * Internal implementation of _setvideomode (.) for the "classical" MDPA,
 * CGA, EGA, & VGA video modes.
 */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (libi86_setvideomode_nonsvga.S.LIBI86)

	.global	__libi86_setvideomode_nonsvga
__libi86_setvideomode_nonsvga:
	ENTER_BX_ (2)
	MOV_ARG0W_BX_ (%ax)
	pushw	%ax
	int	$0x10			/* Set the video mode; %ah == 0 */
	movw	$0x4f03, %ax		/* Try to retrieve the new video */
	int	$0x10			/* mode via VESA, & check it */
	cmpw	$0x004f, %ax
	jnz	.retry
	popw	%ax
	cmpw	%bx,	%ax
	jnz	.error			/* If we successfully set the */
	andb	$0x7f,	%ah		/* video mode, reset the text window */
.ok:					/* & return the text row count, else
					   say 0 */
	TAIL_CALL_ (__libi86_con_mode_changed, 2)
.retry:					/* If using VESA fails, try to */
	movb	$0x0f,	%ah		/* retrieve the new video mode via */
	int	$0x10			/* the traditional interface */
	popw	%dx
	cmp	%dl,	%al
	jnz	.error
	andw	$0x007f, %ax
	jmp	.ok
.error:
	xorw	%ax,	%ax
	RET_ (2)
