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
#include <string.h>
#include <unistd.h>
#include "direct.h"
#include "dos.h"
#include "libi86/stdlib.h"
#include "libi86/internal/dos.h"

static char *
__libi86_getdcwd_maxpath (char buf[_MAX_PATH], unsigned char drive)
{
  /*
   * First get the current directory for the specified drive, sans drive
   * letter.  If that fails, bail out.
   */
  if (__libi86_msdos_do_getdcwd (buf + 3, drive) != 0)
    return NULL;

  /* If there was no failure, get the drive letter. */
  if (! drive)
    drive = _getdrive ();

  /* Plug in the drive letter. */
  buf[0] = 'A' - 1 + drive;
  buf[1] = ':';
  buf[2] = '\\';
  return buf;
}

char *
_getdcwd (int drive, char *buf, size_t size)
{
  if (! buf)
    {
      char our_buf[_MAX_PATH];

      if (! __libi86_getdcwd_maxpath (our_buf, drive))
	return NULL;

      return strdup (our_buf);
    }
  else if (size >= _MAX_PATH)
    return __libi86_getdcwd_maxpath (buf, drive);
  else
    {
      char our_buf[_MAX_PATH];
      size_t our_len;

      if (! __libi86_getdcwd_maxpath (our_buf, drive))
	return NULL;

      our_len = strlen (our_buf);
      if (our_len >= size)
	{
	  errno = ERANGE;
	  return NULL;
	}

      memcpy (buf, our_buf, our_len + 1);
      return buf;
    }
}
