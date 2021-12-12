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
#include <stdlib.h>
#include "dos.h"
#include "libi86/stdlib.h"
#include "libi86/internal/dos.h"

char *
_searchpath (const char *name)
{
  static char full_pathname[_MAX_PATH];

  const char *pathname;
  _dos_dbcs_lead_table_t dbcs;
  __libi86_msdos_path_itr_t itr;
  unsigned attrs;

  if (_dos_getfileattr (name, &attrs) == 0)
    return _fullpath (full_pathname, name, sizeof full_pathname);

  dbcs = _dos_get_dbcs_lead_table ();

  _LIBI86_FOR_EACH_PATHED_PATHNAME (pathname, name, dbcs, itr)
    {
      if (_dos_getfileattr (pathname, &attrs) == 0)
	{
	  _dos_free_dbcs_lead_table (dbcs);
	  return _fullpath (full_pathname, pathname, sizeof full_pathname);
	}
    }

  _dos_free_dbcs_lead_table (dbcs);
  return NULL;
}

#ifdef __GNUC__
_LIBI86_WEAK_ALIAS (_searchpath) char *
searchpath (const char *);
#endif
