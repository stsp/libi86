/*
 * Copyright (c) 2021--2023 TK Chia
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
 * Internal routine for starting an iteration loop over the PATH environment
 * variable.
 */

#define _LIBI86_COMPILING_
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "libi86/internal/cdefs.h"
#include "libi86/internal/dos.h"
#include "libi86/internal/dos-dbcs.h"

char *
__libi86_msdos_pathed_first (const char *name, const char * const *envp,
			     _dos_dbcs_lead_table_t dbcs,
			     __libi86_msdos_path_itr_t *itr)
{
  itr->__name_ = name;
  itr->__dbcs_ = dbcs;
  if (! envp)
    itr->__next_path_ = getenv ("PATH");
  else
    {
      const char *p, * const *pp = envp;
      while ((p = *pp++) != NULL)
	if (p[0] == 'P' && strncmp (p, "PATH=", 5) == 0)
	  {
	    p += 5;
	    break;
	  }
      itr->__next_path_ = p;
    }

  return __libi86_msdos_pathed_next (itr);
}
