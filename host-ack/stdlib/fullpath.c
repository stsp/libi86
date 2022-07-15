/*
 * Copyright (c) 2021 TK Chia
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

#define _LIBI86_COMPILING_
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "direct.h"
#include "dos.h"
#include "libi86/internal/acconfig.h"
#include "libi86/internal/dos.h"
#include "libi86/internal/dos-dbcs.h"

#ifndef _LIBI86_INTERNAL_HAVE_ENAMETOOLONG
# define ENAMETOOLONG	E2BIG
#endif

/*
 * This code was mainly cribbed from my own implementation of realpath (, )
 * for newlib-ia16.  -- tkchia 20211212
 */

#define COPY(chrs, len) \
	do \
	  { \
	    if ((len) >= size || j >= size - (len)) \
	      { \
		errno = ENAMETOOLONG; \
		goto bail; \
	      } \
	    memcpy (out_path + j, (chrs), (len)); \
	    j += (len); \
	  } \
	while (0)
#define COPY1(chr) \
	do \
	  { \
	    if (j >= size - 1) \
	      { \
		errno = ENAMETOOLONG; \
		goto bail; \
	      } \
	    out_path[j++] = (chr); \
	  } \
	while (0)

static int
__libi86_msdos_truename (const char *path, char *out_path, size_t size)
{
  if (size >= _MAX_PATH)
    return __libi86_msdos_do_truename (path, out_path) ? -1 : 0;
  else
    {
      char buf[_MAX_PATH];
      size_t len;

      unsigned res = __libi86_msdos_do_truename (buf, out_path);
      if (res)
	return -1;

      len = strlen (buf);
      if (len > size)
	{
	  errno = ENAMETOOLONG;
	  return -1;
	}

      memcpy (out_path, buf, len + 1);
      return 0;
    }
}

static size_t
__libi86_msdos_dbcs_strcspn (const char *s, char reject1, char reject2,
			     _dos_dbcs_lead_table_t dbcs)
{
  const char *p = s;
  char c;

  for (;;)
    {
      c = *p;
      if (! c || c == reject1 || c == reject2)
	break;

      ++p;
      if (__libi86_msdos_dbcs_lead_p (c, dbcs))
	{
	  if (! *p)
	    break;
	  ++p;
	}
    }

  return (size_t) (p - s);
}

char *
_fullpath (char *out_path, const char *path, size_t size)
{
  size_t i = 0, j = 0;
  bool out_path_alloced = false;
  unsigned char drive;
  char c;
  /*
   * To keep track of the number of path components in the output, & where
   * each component begins (& ends).
   */
  size_t n_comps, comp_start[_MAX_PATH / 2];
  _dos_dbcs_lead_table_t dbcs = _null_dbcs_lt;

  if (size < 2)
    {
      errno = ENAMETOOLONG;
      return NULL;
    }

  if (! path || ! path[0])
    path = ".";

  if (size > _MAX_PATH)
    size = _MAX_PATH;

  if (! out_path)
    {
      size = _MAX_PATH;
      out_path = malloc (size);
      if (! out_path)
	return NULL;
      out_path_alloced = true;
    }

  /*
   * If the path is a network path, then just do a "truename" syscall & call
   * it a day.
   */
  if (path[1] == '\\' && path[0] == '\\')
    {
      if (__libi86_msdos_truename (path, out_path, size) == 0)
	return out_path;
      goto bail;
    }

  dbcs = _dos_get_dbcs_lead_table ();

  /* Not a network path.  Process any drive letter. */
  if (path[1] == ':' && ! __libi86_msdos_dbcs_lead_p (path[0], dbcs))
    {
      drive = path[0];
      if (__libi86_msdos_drive_letter_p (drive))
	{
	  if (drive >= 'A' && drive <= 'Z')
	    drive -= 'A' - 1;
	  else
	    drive -= 'a' - 1;
	}
      else
	goto invalid;

      i = 2;
    }
  else
    drive = _getdrive ();

  /*
   * Is the first character (after any drive letter) a path separator?   If
   * not, then try to find the working directory of the given drive.  If that
   * fails, arrange to return a relative path.
   */
  if (drive)
    {
      if (__libi86_msdos_path_sep_p (path[i]))
	{
	  out_path[j++] = drive - 1 + 'A';
	  out_path[j++] = ':';
	  out_path[j++] = '\\';
	  ++i;
	  n_comps = 1;
	  comp_start[0] = j;
	}
      else if (_getdcwd (drive, out_path, size))
	{
	  char *p = out_path;
	  n_comps = 0;
	  while ((c = *p++) != 0)
	    {
	      if (__libi86_msdos_dbcs_lead_p (c, dbcs))
		{
		  if (! *p++)
		    break;
		}
	      else if (__libi86_msdos_path_sep_p (c))
		{
		  comp_start[n_comps] = p - out_path;
		  ++n_comps;
		}
	    }
	  /*
	   * p now points after the null terminator from the obtained
	   * current directory.  We add a trailing backslash if the last
	   * (possibly multi-byte) character seen was not a path separator.
	   */
	  j = p - 1 - out_path;
	  if (! n_comps || comp_start[n_comps - 1] != j)
	    {
	      out_path[j++] = '\\';
	      comp_start[n_comps] = j;
	      ++n_comps;
	    }
	}
      else
	{
	  out_path[j++] = drive - 1 + 'A';
	  out_path[j++] = ':';
	  n_comps = 1;
	  comp_start[0] = j;
	}
    }

  /* Process the rest of the path, component by component. */
  while (path[i] != 0)
    {
      struct _fcb fcb;
      size_t name_len, ext_len;

      size_t k = __libi86_msdos_dbcs_strcspn (path + i, '\\', '/', dbcs), m;
      switch (k)
	{
	case 0:
	  ++i;
	  continue;

	case 1:
	  if (path[i] == '.')
	    {
	      ++i;
	      continue;
	    }
	  break;

	case 2:
	  if (path[i] == '.' && path[i + 1] == '.')
	    {
	      i += 2;
	      if (n_comps > 1)
		{
		  j = comp_start[n_comps - 2];
		  --n_comps;
		}
	      else
		{
		  j = comp_start[0];
		  COPY ("..\\", 3);
		  comp_start[0] = j;
		}
	      continue;
	    }
	  break;

	default:
	  ;
	}

      if (_parsfnm (path + i, &fcb, 0) != path + i + k
	  || fcb._fcb_drive != 0)
	goto invalid;

      i += k;

      name_len = m = 0;
      do
	{
	  c = (char) fcb._fcb_name[m];
	  ++m;
	  if (c != ' ')
	    {
	      if (m < 8 && __libi86_msdos_dbcs_lead_p (c, dbcs))
		++m;
	      name_len = m;
	    }
	}
      while (m < 8);

      ext_len = m = 0;
      do
	{
	  c = (char) fcb._fcb_ext[m];
	  ++m;
	  if (c != ' ')
	    {
	      if (m < 3 && __libi86_msdos_dbcs_lead_p (c, dbcs))
		++m;
	      ext_len = m;
	    }
	}
      while (m < 3);

      if (! name_len)
	goto invalid;

      COPY (fcb._fcb_name, name_len);
      if (ext_len)
	{
	  COPY1 ('.');
	  COPY (fcb._fcb_ext, ext_len);
	}
      COPY1 ('\\');
      comp_start[n_comps] = j;
      ++n_comps;
    }

  /* We are done. */
  if (j > 3)
    out_path[j - 1] = 0;
  else
    out_path[j] = 0;

  return out_path;

invalid:
  errno = EINVAL;
bail:
  if (out_path_alloced)
    free (out_path);
  return NULL;
}
