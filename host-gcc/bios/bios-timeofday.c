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

#define _LIBI86_COMPILING_
#include "bios.h"

unsigned short
_bios_timeofday (unsigned service, long *timeval)
{
  unsigned char midnight;
  unsigned tv_hi, tv_lo;
  unsigned xx1, xx2, xx3;
  long tv;
  switch (service)
    {
    case _TIME_GETCLOCK:
      __asm volatile ("int $0x1a"
		      : "=Ral" (midnight), "=c" (tv_hi), "=d" (tv_lo)
		      : "Rah" ((unsigned char) 0)
		      : "cc", "memory");
      *timeval = (long) tv_hi << 16 | tv_lo;
      return (unsigned short) midnight;

    case _TIME_SETCLOCK:
      tv = *timeval;
      tv_hi = (unsigned) (tv >> 16);
      tv_lo = (unsigned) tv;
      __asm volatile ("int $0x1a"
		      : "=a" (xx1), "=c" (xx2), "=d" (xx3)
		      : "Rah" ((unsigned char) 1), "1" (tv_hi), "2" (tv_lo)
		      : "cc", "memory");
      return 0;
    }

  return (unsigned short) -1;
}
