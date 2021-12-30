#
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

#include "libi86/internal/sect.h"

/*
 * As of Oct 2021, ACK's C runtime library lacks a vsscanf (...) function,
 * though it does have sscanf (...).
 *
 * I try to implement vsscanf (...) in terms of sscanf (...): first (over-)
 * estimate the size of the va_list'd argument list passed to vsscanf (...),
 * then copy these variadic arguments & pass them to sscanf (...).  This is
 * relatively easy to do for ...scanf (...), since the argument list will
 * comprise only pointers.
 *
 * (We could alternatively implement vsscanf (...) in terms of ACK's
 * internal code & data structures, but this may break more easily.)
 */

	.define	__vsscanf
__vsscanf:
	push	si
	push	di
	push	bp
	mov	bp, sp
	mov	si, 10(bp)		! get format string
	xor	cx, cx			! cx := no. of variadic arguments
.loop:					! scan format string for `%' characters
	lodsb
	cmpb	al, '%'
	ja	.loop
	jnz	.next
	inc	cx			! if we see a `%', assume we have
	lodsb				! one more argument
.next:
	testb	al, al
	jnz	.loop
	mov	si, 12(bp)		! we scanned the format string; now
	mov	di, sp			! copy arguments
	sub	di, cx
	sub	di, cx
	mov	sp, di
	rep movsw
	push	10(bp)			! push the remaining arguments that
	push	8(bp)			! sscanf (...) needs
	call	_sscanf			! ...& call
	mov	sp, bp			! we are done
	pop	bp
	pop	di
	pop	si
	ret
