/*
 * Copyright (c) 2021--2022 TK Chia
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

#ifndef _LIBI86_LIBI86_INTERNAL_BC_PORTIO_H_
#define _LIBI86_LIBI86_INTERNAL_BC_PORTIO_H_

#if ! defined _LIBI86_COMPILING && defined _BORLANDC_SOURCE
# include <libi86/internal/portio.h>
# include <libi86/internal/inp-portio.h>

_LIBI86_BEGIN_EXTERN_C

_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, inportb, unsigned, __libi86_inp)
_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, inportw, unsigned, __libi86_inpw)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, outportb, unsigned, unsigned,
                               __libi86_outp)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, outportw, unsigned, unsigned,
                               __libi86_outpw)
/*
 * In the Borland toolchain, inport (.) & outport (, ) are older versions
 * of the inportw (.) & outportw (, ) functions that deal with signed values
 * rather than unsigned ones.
 */
_LIBI86_REDIRECT_AND_INLINE_1 (int, inport, unsigned,
			       __libi86_inpw_signed)
_LIBI86_REDIRECT_AND_INLINE_VOID_2 (outport, unsigned, int,
				    __libi86_outpw_signed)

_LIBI86_END_EXTERN_C

#endif /* ! _LIBI86_COMPILING && _BORLANDC_SOURCE */

#endif
