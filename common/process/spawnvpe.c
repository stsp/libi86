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
#include "dos.h"
#include "process.h"
#include "libi86/internal/dos.h"
#include "libi86/internal/dos-dbcs.h"

static bool
is_specific_path (const char *path, _dos_dbcs_lead_table_t dbcs)
{
  bool led_p = false;
  const char *p = path;
  char c;
  if (__libi86_msdos_has_drive_spec_p (path))
    return true;
  while ((c = *p++) != 0)
    {
      if (led_p)
	led_p = false;
      else if (__libi86_msdos_path_sep_p (c))
	return true;
      else if (__libi86_msdos_dbcs_lead_p (c, dbcs))
	led_p = true;
    }
  return false;
}

__libi86_pid_t
_spawnvpe (int mode, const char *path, const char * const *argv,
	   const char * const *envp)
{
  _dos_dbcs_lead_table_t dbcs;
  __libi86_msdos_path_itr_t itr;
  __libi86_pid_t res;
  const char *full;

  if (! path || ! path[0])
    {
      errno = EINVAL;
      return -1;
    }

  res = _spawnve (mode, path, argv, envp);
  if (res != -1 || errno != ENOENT)
    return res;

  dbcs = _dos_get_dbcs_lead_table ();
  if (is_specific_path (path, dbcs))
    return res;

  _LIBI86_FOR_EACH_PATHED_PATHNAME (full, path, envp, dbcs, itr)
    {
      res = _spawnve (mode, full, argv, envp);
      if (res != -1 || errno != ENOENT)
	return res;
    }

  return res;
}

#ifdef __GNUC__
_LIBI86_WEAK_ALIAS (_spawnvpe) __libi86_pid_t
spawnvpe (int, const char *, const char * const *, const char * const *);
#endif
