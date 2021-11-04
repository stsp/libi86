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

#define _LIBI86_COMPILING_
#include <sys/time.h>
#include "i86.h"

/* FIXME: this implementation is way less efficient than it can be. */

void
delay (unsigned ms)
{
  struct timeval tv_start, tv;
  time_t s_diff;
  suseconds_t us, us_diff;

  if (! ms)
    return;

  if (gettimeofday (&tv_start, NULL) != 0)
    return;

  while (ms >= 1000U)
    {
      do
	{
	  if (gettimeofday (&tv, NULL) != 0)
	    return;

	  s_diff = tv.tv_sec - tv_start.tv_sec;
	} while (s_diff < 1 || s_diff == 1 && tv.tv_usec < tv_start.tv_usec);

      ++tv_start.tv_sec;
      ms -= 1000U;
    }

  if (! ms)
    return;

  us = (suseconds_t) ms * 1000L;

  do
    {
      if (gettimeofday (&tv, NULL) != 0)
	return;

      s_diff = tv.tv_sec - tv_start.tv_sec;
      us_diff = tv.tv_usec - tv_start.tv_usec;

      if (s_diff > 0)
	us_diff += 1000000L;
    } while (us_diff < us);
}
