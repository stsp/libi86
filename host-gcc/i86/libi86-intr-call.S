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

/*
 * This file will be compiled with intr_no spanning values from 0000 to 0377
 * (numerically, 0x00 to 0xff).
 *
 * All the routines here are near routines in the default text segment
 * (.text).  The external routines that reference these (__libi86_int86_do
 * (...) etc.) should also go into .text, even if they are far, so that they
 * can directly invoke the routines here.
 */

#APP
#define P_(x, y)	P2_ (x, y)
#define P2_(x, y)	x ## y

	.code16
	.att_syntax prefix

	.text
	.global	P_ (__libi86_intr_call_, intr_no)
P_ (__libi86_intr_call_, intr_no):
#if intr_no == 3
	int3
	ret
#else
	int	$(intr_no)
# ifdef __MSDOS__
#   if intr_no == 0x25 || intr_no == 0x26
	jc	0f
	popfw
	clc
	ret
0:
	popfw
	stc
#   endif
# endif
	ret
#endif
