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
#include <stdbool.h>
#include <string.h>
#include "libi86/stdlib.h"
#include "libi86/internal/dos.h"

#ifdef __MSDOS__
# define ADD_CHAR(x) \
		do \
		  { \
		    if (p == e) \
		      goto overflow; \
		    *p++ = (x); \
		  } \
		while (0)
# define ADD(comp, add_path_sep_p) \
		do \
		  { \
		    bool __add_path_sep_p = (add_path_sep_p); \
		    const char *__comp = (comp); \
		    if (__comp && *__comp) \
		      { \
			char __c; \
			while ((__c = *__comp++) != 0) \
			  { \
			    ADD_CHAR (__c); \
			    if (__add_path_sep_p) \
			      { \
				if (__libi86_msdos_path_sep_p (__c)) \
				  { \
				    last_path_sep = __c; \
				    last_c_was_path_sep_p = true; \
				  } \
				else \
				  last_c_was_path_sep_p = false; \
			      } \
			  } \
			if (__add_path_sep_p && ! last_c_was_path_sep_p) \
			  ADD_CHAR (last_path_sep); \
		    } \
		  } \
		while (0)

int
_makepath (char path[_MAX_PATH], const char *drive, const char *dir,
	   const char *fname, const char *ext)
{
  char *p = path, *e;
  bool last_c_was_path_sep_p = false;
  /*
   * Open Watcom C tries to use the same path separator character as used in
   * the original path.  This code tries to do something similar, but a bit
   * differently.  OW keeps track of the _first_ path separator it sees,
   * while this code keeps track of the last.
   */
  char last_path_sep = '\\';

  if (! path)
    goto bogus0;

  e = &path[_MAX_PATH - 1];

  /*
   * If there is a drive letter or volume identifier, copy it.  Add an
   * obligatory trailing `:' or `\\' if necessary.
   */
  if (drive && *drive)
    {
      char d0 = drive[0], d1 = drive[1];
      if (__libi86_msdos_drive_letter_p (d0))
	{
	  if (! d1)
	    {
	      *p++ = d0;
	      *p++ = ':';
	    }
	  else if (d1 == ':' && ! drive[2])
	    {
	      *p++ = d0;
	      *p++ = d1;
	    }
	  else
	    goto bogus;
	}
      else
	goto bogus;
    }

  /* If there is a directory path component, copy it. */
  ADD (dir, true);

  /* Ditto for the file name component. */
  ADD (fname, false);

  /* And the extension. */
  if (ext && *ext)
    {
      if (*ext != '.')
	ADD_CHAR ('.');
      ADD (ext, false);
    }

  /* If we are done & there are no errors, add the terminator byte. */
  *p = 0;
  return 0;

bogus:
  path[0] = 0;
bogus0:
  errno = EINVAL;
  return -1;

overflow:
  *e = 0;
# ifdef _LIBI86_INTERNAL_HAVE_ENAMETOOLONG
  errno = ENAMETOOLONG;
# else
  errno = E2BIG;  /* FIXME */
# endif
  return -1;
}
#else
# pragma GCC warning "unknown host OS"
#endif
