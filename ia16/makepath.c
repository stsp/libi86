/*
 * Copyright (c) 2021 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING2.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
 */

#define _LIBI86_COMPILING_
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include "libi86/stdlib.h"
#include "libi86/internal/path.h"

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
  errno = ENAMETOOLONG;
  return -1;
}
#else
# warning "unknown host OS"
#endif
