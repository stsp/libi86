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
 * CGA, & EGA video modes.
 */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"
#include "graph.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (libi86_setvideomode_default.S.LIBI86)

.Lctor_setvideomode:
	movw	__libi86_vid_state, %ax	/* Get the initial video mode */
	testw	$~0x007f, %ax		/* If this is not a classical video */
	jz	.good			/* mode, then to simplify things we */
	movb	$_TEXTC80, %al		/* treat _TEXTC80 as the "default"
					   mode to return to (in fact Open
					   Watcom does not really handle
					   this case!) */
.good:					/* If a classical mode, just */
	movb	%al,	.Ldef_mode	/* remember the actual mode */
	RET_ (0)

	.global	__libi86_setvideomode_default
__libi86_setvideomode_default:
	movb	.Ldef_mode, %al
	cbtw
#ifdef __IA16_CALLCVT_REGPARMCALL
	JMP_ (__libi86_setvideomode_nonsvga)
#else
	pushw	%ax
	CALL_ (__libi86_setvideomode_nonsvga)
# ifndef __IA16_CALLCVT_STDCALL
	popw	%dx
# endif
	RET_ (0)
#endif

	.section .ctors.65435

	.balign	2
	TEXT_PTR_ (.Lctor_setvideomode)

	.lcomm	.Ldef_mode, 1
