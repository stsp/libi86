/*
 * Copyright (c) 2019 TK Chia
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

#include "libi86/internal/acconfig.h"
#include "libi86/internal/call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (osmajor_osminor.S.LIBI86)
.Lctor_osmajor_osminor:
	movb	$0x30,	%ah
	int	$0x21
	movw	%ax,	0f
	RET_(0)

	.section .ctors.65535

	.balign	2
	TEXT_PTR_ (.Lctor_osmajor_osminor)

	.bss

/* The variables must be arranged like this, so that a single store of %ax
   can correctly fill both {__libi86_}_osmajor and {__libi86_}_osminor.  */
0:
#ifndef _LIBI86_INTERNAL_HAVE__OSMAJOR
	.global	_osmajor
_osmajor:
#endif
	.global	__libi86_osmajor
__libi86_osmajor:
	.skip	1
#ifndef _LIBI86_INTERNAL_HAVE__OSMINOR
	.global	_osminor
_osminor:
#endif
	.global	__libi86_osminor
__libi86_osminor:
	.skip	1
