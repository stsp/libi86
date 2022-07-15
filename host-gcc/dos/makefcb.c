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

#define _LIBI86_COMPILING_
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dos.h"
#ifdef __IA16_FEATURE_DOSX
# include "dpmi.h"
# include "libi86/string.h"
#endif

#ifdef __IA16_FEATURE_DOSX
/*
 * Notes:
 *
 *  * MS-DOS only frobs the FCB up to the logical record size
 *    (._fcb_recsize) field.  I am paranoid though, so this code copies the
 *    entire `struct _fcb' to & from the base memory transfer buffer, when
 *    in DOS extender mode.  This is in case future DOS clones want to
 *    implement any new OPT-ions & do additional stuff on the FCB.
 *
 *  * The code here treats a null or CR as terminating the CMD_LINE string,
 *    when deciding how much of it to copy to the transfer buffer.  (LF is
 *    also recognized, for good measure.)
 *
 *    This takes care of the most common use cases for _makefcb (...):
 *    parsing an ASCIIZ file name, & parsing a command line to pass to int
 *    0x21, ah = 0x4b.
 *
 *    MS-DOS 2.x in fact stops scanning the CMD_LINE once it hits
 *	* a control character, which it defines as any character with code
 *	  0--31; or
 *	* a terminator (?) character which is not also a one-time delimiter
 *	  --- '"', '/', '\\', '[', or ']'.
 */
typedef struct __libi86_packed
{
  struct _fcb fcb;
  char cmd_line[];
} makefcb_t;

static size_t
cmd_line_len (const char *cmd_line)
{
  const char *p = cmd_line;
  char c;
  bool termr = false;
  do
    {
      c = *p++;
      switch (c)
	{
	case 0:
	case '\r':
	case '\n':
	  termr = true;
	}
    }
  while (! termr);
  return p - cmd_line;
}
#endif  /* __IA16_FEATURE_DOSX */

struct _makefcb_t
_makefcb (const char *cmd_line, __libi86_fcb_ptr_t p_fcb, int opt)
{
  struct _fcb *pf = (struct _fcb *) p_fcb;
  char *tail;
  int status;

#ifndef __IA16_FEATURE_DOSX
  uint16_t ax;

  __asm volatile ("int $0x21"
		  : "=a" (ax), "=S" (tail)
		  : "Rah" ((uint8_t) 0x29), "Ral" ((uint8_t) opt),
		    "Rds" (FP_SEG (cmd_line)), "1" (FP_OFF (cmd_line)),
		    "e" (FP_SEG (pf)), "D" (FP_OFF (pf))
		  : "cc", "memory");

  status = (int8_t) ax;
  return (struct _makefcb_t) { status, tail };
#else  /* __IA16_FEATURE_DOSX */
  const size_t PARA_SZ = 0x10;
  size_t len;
  dpmi_dos_block blk;
  makefcb_t __far *parse;
  rm_call_struct rmc;

  len = cmd_line_len (cmd_line);

  if (len + sizeof (makefcb_t) <= sizeof (makefcb_t))
    return (struct _makefcb_t) { -1, NULL };

  blk = _DPMIAllocateDOSMemoryBlock ((len + sizeof (makefcb_t) - 1)
				     / PARA_SZ + 1);
  if (! blk.pm)
    return (struct _makefcb_t) { -1, NULL };

  parse = (makefcb_t __far *) MK_FP (blk.pm, 0);
  parse->fcb = *pf;
  _fmemcpy (parse->cmd_line, cmd_line, len);

  memset (&rmc, 0, sizeof (rmc));  /* also set rmc.ss = rmc.sp := 0 */
  rmc.ax = 0x2900 | (uint8_t) opt;
  rmc.ds = rmc.es = blk.rm;
  rmc.si = offsetof (makefcb_t, cmd_line);
  rmc.di = offsetof (makefcb_t, fcb);

  status = _DPMISimulateRealModeInterrupt (0x21, 0, 0, &rmc);
  if (status != 0)
    return (struct _makefcb_t) { -1, NULL };

  *pf = parse->fcb;
  _DPMIFreeDOSMemoryBlock (blk.pm);

  status = (int8_t) rmc.ax;
  tail = (char *) cmd_line + (rmc.si - offsetof (makefcb_t, cmd_line));
  return (struct _makefcb_t) { status, tail };
#endif  /* __IA16_FEATURE_DOSX */
}
