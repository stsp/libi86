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
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "dos.h"
#include "libi86/string.h"
#include "libi86/internal/dos.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

#define POSIX_O_ACCMODE	((unsigned) (O_RDONLY | O_WRONLY | O_RDWR))
#define DOS_BAD_ACCESS	12  /* MS-DOS error code for "invalid access" */

unsigned
_dos_open (const char *path, unsigned mode, int *handle)
{
  /*
   * In case the C library's O_RDONLY, O_WRONLY, &/or O_RDWR values do not
   * match up with MS-DOS's...
   */
  if (O_RDONLY != 0 && (mode & POSIX_O_ACCMODE) == O_RDONLY)
    mode = (mode & ~(POSIX_O_ACCMODE | 3U)) | 0U;
  else if (O_WRONLY != 1 && (mode & POSIX_O_ACCMODE) == O_WRONLY)
    mode = (mode & ~(POSIX_O_ACCMODE | 3U)) | 1U;
  else if (O_RDWR != 2 && (mode & POSIX_O_ACCMODE) == O_RDWR)
    mode = (mode & ~(POSIX_O_ACCMODE | 3U)) | 2U;

  if (mode >> 8 != 0)
    return __libi86_ret_really_set_errno (DOS_BAD_ACCESS);

#ifdef __IA16_FEATURE_PROTECTED_MODE
  if (__DPMI_hosted () == 1)
    {
      dpmi_dos_block path_blk;
      rm_call_struct rmc;
      int res;

      path_blk = __libi86_dpmi_low_dup_str (path);
      if (! path_blk.pm)
	return errno;

      rmc.ss = rmc.sp = rmc.flags = 0;
      rmc.ax = 0x3d00U | mode;
      rmc.ds = path_blk.rm;
      rmc.dx = 0;
      res = _DPMISimulateRealModeInterrupt (0x21, 0, 0, &rmc);

      __libi86_dpmi_low_free_str (path_blk);

      if (res != 0)
	{
	  errno = res = EIO;
	  return res;
	}

      if ((rmc.flags & 1) != 0)
	{
	  errno = res = rmc.ax;
	  return res;
	}

      *handle = rmc.ax;
      return 0;
    }
  else
#endif
    {
      unsigned ax, res, xx1, xx2;

      __asm volatile ("int $0x21; sbbw %1, %1"
		      : "=a" (ax), "=bc" (res), "=bc" (xx1), "=d" (xx2)
		      : "Rah" ((unsigned char) 0x3d),
			"Ral" ((unsigned char) mode),
			"Rds" (FP_SEG (path)), "3" (FP_OFF (path))
		      : "cc", "memory");

      if (res)
	return __libi86_ret_really_set_errno (ax);

      *handle = ax;
      return 0;
    }
}
