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
#include <string.h>
#include "libi86/stdlib.h"
#include "libi86/internal/dos.h"
#include "libi86/internal/dos-dbcs.h"

#ifdef __MSDOS__
static const char *
copy_component (const char *p, size_t pn, char *buf, size_t bn)
{
  if (buf)
    {
      if (pn < bn)
	bn = pn + 1;
      memcpy (buf, p, bn - 1);
      buf[bn - 1] = 0;
    }

  return p + pn;
}

static void
skip_component (char *buf)
{
  if (buf)
    buf[0] = 0;
}

void
_splitpath (const char *path, char drive[_MAX_DRIVE], char dir[_MAX_DIR],
	    char fname[_MAX_FNAME], char ext[_MAX_EXT])
{
  _dos_dbcs_lead_table_t dbcs = _dos_get_dbcs_lead_table ();
  const char *p = path, *q, *dir_end = NULL, *name_end = NULL;
  char c;

  /* If there is a drive letter, extract it. */
  if (__libi86_msdos_has_drive_spec_p (p))
    p = copy_component (p, 2, drive, _MAX_DRIVE);
  else
    skip_component (drive);

  /* If there is a directory path component, extract it. */
  q = p;
  while ((c = *q++) != 0)
    {
      if (__libi86_msdos_path_sep_p (c))
	dir_end = q;
      else if (__libi86_msdos_dbcs_lead_p (c, dbcs) && *q)
	++q;
    }
  if (dir_end)
    p = copy_component (p, dir_end - p, dir, _MAX_DIR);
  else
    skip_component (dir);

  /* Extract the file name & extension. */
  q = p;
  while ((c = *q++) != 0)
    {
      if (c == '.')
	name_end = q - 1;
      else if (__libi86_msdos_dbcs_lead_p (c, dbcs) && *q)
	++q;
    }
  if (name_end)
    {
      p = copy_component (p, name_end - p, fname, _MAX_FNAME);
      copy_component (p, strlen (p), ext, _MAX_EXT);
    }
  else
    {
      copy_component (p, strlen (p), fname, _MAX_FNAME);
      skip_component (ext);
    }
}
#else
# pragma GCC warning "unknown host OS"
#endif
