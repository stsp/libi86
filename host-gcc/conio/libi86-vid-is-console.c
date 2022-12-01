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

/*
 * When running under ELKS, decide whether the controlling terminal is
 * indeed a console device.
 *
 * Assume it is, if it supports the DCGET_GRAPH & possibly DCREL_GRAPH ioctl's.
 */

#define _LIBI86_COMPILING_
#include "libi86/internal/conio.h"
#include <linuxmt/ntty.h>

bool __libi86_vid_is_console = false;

__attribute__ ((constructor (101))) static void
__libi86_vid_is_console_init (void)
{
  int fd = __libi86_con_out_fd;
  int res = __libi86_tty_ioctl (fd, DCGET_GRAPH, NULL);
  if (res >= 0)
    {
      if (__libi86_tty_ioctl (fd, DCREL_GRAPH, NULL) >= 0)
	__libi86_vid_is_console = true;
    }
  else if (res == -EBUSY)
    __libi86_vid_is_console = true;
}
