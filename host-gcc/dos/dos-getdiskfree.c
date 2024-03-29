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
#include <errno.h>
#include "dos.h"
#include "libi86/internal/dos.h"

unsigned
_dos_getdiskfree (unsigned drive, struct diskfree_t *diskspace)
{
  unsigned tot, avail, spc, bps;

  if (drive > 0xff)
    return (unsigned) __libi86_ret_einval ();

  __asm volatile ("int $0x21"
		  : "=a" (spc), "=b" (avail), "=c" (bps), "=d" (tot)
		  : "Rah" ((unsigned char) 0x36), "Rdl" ((unsigned char) drive)
		  : "cc");

  if (spc == 0xffff)
    return (unsigned) __libi86_ret_einval ();

  diskspace->total_clusters = tot;
  diskspace->avail_clusters = avail;
  diskspace->sectors_per_cluster = spc;
  diskspace->bytes_per_sector = bps;
  return 0;
}
