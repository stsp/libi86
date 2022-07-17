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

/* Internal routines for iterating over the PATH environment variable. */

#define _LIBI86_COMPILING_
#include <errno.h>
#include <stdlib.h>
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

char *
__libi86_msdos_pathed_next (__libi86_msdos_path_itr_t *itr)
{
  bool last_c_lead_p = false, last_c_path_sep_p = false;
  char c, last_path_sep = '\\', *q = itr->__pathname_,
       *e = itr->__pathname_ + sizeof itr->__pathname_ - 1;
  const char *next_path = itr->__next_path_, *p;

  if (! next_path)
    {
      errno = ENOENT;
      return NULL;
    }

  /* Copy out one search directory from %PATH%. */
  p = next_path;
  while ((c = *p++) != 0 && c != ';')
    {
      if (q == e)
	goto too_long;
      *q++ = c;
      if (last_c_lead_p)
	last_c_lead_p = false;
      else if (itr->__dbcs_ && __libi86_msdos_dbcs_lead_p (c, itr->__dbcs_))
	last_c_lead_p = true;
      else if (__libi86_msdos_path_sep_p (c))
	{
	  last_c_path_sep_p = true;
	  last_path_sep = c;
	}
      else
	last_c_path_sep_p = false;
    }

  if (last_c_lead_p)
    {
      errno = EILSEQ;
      return NULL;
    }

  if (c)
    next_path = p;
  else
    next_path = NULL;

  /*
   * If the last character in the %PATH% search directory was not a path
   * separator, & the directory was not simply a drive letter specifier,
   * then we need to append a path separator.
   */
  if (! last_c_path_sep_p)
    {
      if (p != next_path + 3 || ! __libi86_msdos_has_drive_spec_p (next_path))
	{
	  if (q == e)
	    goto too_long;
	  *q++ = last_path_sep;
	}
    }

  /* Append the file name component. */
  p = itr->__name_;
  while ((c = *p++) != 0)
    {
      if (q == e)
	goto too_long;
      *q++ = c;
    }
  *q++ = 0;

  /* Update *ITR for the next iteration. */
  itr->__next_path_ = next_path;

  /* Return the qualified name. */
  return itr->__pathname_;

too_long:
#ifdef _LIBI86_INTERNAL_HAVE_ENAMETOOLONG
  errno = ENAMETOOLONG;
#else
  errno = E2BIG;  /* FIXME */
#endif
  return NULL;
}
