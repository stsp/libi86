/*
 * Copyright (c) 2018--2022 TK Chia
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
 * inp (.) inpw (.), outp (, ), & outpw (, ) is declared by both <dos.h> & 
 * <conio.h> in _BORLANDC_SOURCE mode, but is only declared by <conio.h>
 * in non-Borland mode.  This file is included by both <dos.h> & <conio.h>
 * as appropriate.
 */

#ifndef _LIBI86_LIBI86_INTERNAL_INP_PORTIO_H_
#define _LIBI86_LIBI86_INTERNAL_INP_PORTIO_H_

#ifndef _LIBI86_COMPILING_
# include <libi86/internal/cdefs.h>
# include <libi86/internal/portio.h>

_LIBI86_BEGIN_EXTERN_C

_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, inp, unsigned, __libi86_inp)
_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, _inp, unsigned, __libi86_inp)
_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, inpw, unsigned, __libi86_inpw)
_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, _inpw, unsigned, __libi86_inpw)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, outp, unsigned, unsigned,
			       __libi86_outp)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, _outp, unsigned, unsigned,
			       __libi86_outp)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, outpw, unsigned, unsigned,
			       __libi86_outpw)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, _outpw, unsigned, unsigned,
			       __libi86_outpw)

_LIBI86_END_EXTERN_C

#endif /* ! _LIBI86_COMPILING */

#endif
