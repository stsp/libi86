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

#ifndef _LIBI86_LIBI86_INTERNAL_BDA_H_
#define _LIBI86_LIBI86_INTERNAL_BDA_H_

#include <stdint.h>
#include <libi86/internal/cdefs.h>
#include <libi86/internal/farptr.h>
#include <graph.h>

_LIBI86_BEGIN_EXTERN_C

/* Manipulating the data area for an IBM-compatible BIOS. */

extern uint16_t __libi86_bda_seg;

_LIBI86_STATIC_INLINE uint16_t
__libi86_peek_bda (unsigned off)
{
  return (uint16_t) __libi86_peek (__libi86_bda_seg, off);
}

_LIBI86_STATIC_INLINE uint8_t
__libi86_peekb_bda (unsigned off)
{
  return (uint8_t) __libi86_peekb (__libi86_bda_seg, off);
}

_LIBI86_STATIC_INLINE void
__libi86_poke_bda (unsigned off, uint16_t val)
{
  __libi86_poke (__libi86_bda_seg, off, (int) val);
}

_LIBI86_END_EXTERN_C

#endif
