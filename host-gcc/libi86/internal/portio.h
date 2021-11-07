/*
 * Copyright (c) 2018 TK Chia
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

#ifndef _LIBI86_LIBI86_INTERNAL_PORTIO_H_
#define _LIBI86_LIBI86_INTERNAL_PORTIO_H_

#include <libi86/internal/acconfig.h>
#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

/*
 * I did not add an "N" constraint for immediate port numbers to gcc-ia16
 * until late October 2018, so some versions of gcc-ia16 may not understand
 * it.  For those, I use a combination of integer constraints that together
 * cover the range 0 ... 0xff of possible immediate port numbers.  -- tkchia
 */
#if _LIBI86_HAVE_ASM_N_CONSTRAINT - 0
# define _LIBI86_IO_PORT_CONSTRAINT "Nd"
#else
# define _LIBI86_IO_PORT_CONSTRAINT "LomZLbmd"
#endif

_LIBI86_ALT_INLINE
unsigned __libi86_inp (unsigned __port)
{
  unsigned char __val;
  __asm volatile ("{inb %1, %0|in %0, %1}"
		  : "=Ral" (__val)
		  : _LIBI86_IO_PORT_CONSTRAINT (__port));
  return (unsigned) __val;
}

_LIBI86_ALT_INLINE
unsigned __libi86_inpw (unsigned __port)
{
  unsigned __val;
  __asm volatile ("{inw %1, %0|in %0, %1}"
		  : "=a" (__val)
		  : _LIBI86_IO_PORT_CONSTRAINT (__port));
  return __val;
}

_LIBI86_ALT_INLINE
int __libi86_inpw_signed (unsigned __port)
{
  return (int) __libi86_inpw (__port);
}

_LIBI86_ALT_INLINE
unsigned __libi86_outp (unsigned __port, unsigned __val)
{
  __asm volatile ("{outb %1, %0|out %0, %1}"
		  : /* no outputs */
		  : _LIBI86_IO_PORT_CONSTRAINT (__port),
		    "Ral" ((unsigned char) __val));
  return __val;
}

_LIBI86_ALT_INLINE
unsigned __libi86_outpw (unsigned __port, unsigned __val)
{
  __asm volatile ("{outw %1, %0|out %0, %1}"
		  : /* no outputs */
		  : _LIBI86_IO_PORT_CONSTRAINT (__port), "a" (__val));
  return __val;
}

_LIBI86_ALT_INLINE
void __libi86_outpw_signed (unsigned __port, int __val)
{
  __libi86_outpw (__port, (unsigned) __val);
}

_LIBI86_END_EXTERN_C

#endif
