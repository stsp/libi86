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

/*
 * The argument quoting logic in this implementation of _spawnve (...) is
 * based on that of DJGPP, but (hopefully) simplified.
 */

#define _LIBI86_COMPILING_
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include "dos.h"
#include "process.h"
#include "libi86/stdlib.h"
#include "libi86/string.h"
#include "libi86/internal/dos.h"
#include "libi86/internal/dos-dbcs.h"
#include "libi86/internal/features.h"
#include "libi86/internal/process.h"

extern char **environ;

typedef struct
{
#ifndef __ACK
  unsigned char
#else
  unsigned
#endif
    f0 : 1, f1 : 1, f2 : 1, f3 : 1, f4 : 1, f5 : 1, f6 : 1, f7 : 1;
} flags_t;
typedef __libi86_pid_t
	(*spawner_t) (int, unsigned char,
		      const char *, const char [8], const char[3],
		      const char * const *, const char * const *,
		      _dos_dbcs_lead_table_t);

static bool
is_dos_shell (const char name[8], const char ext[3])
{
  static const char command_name[8] = "COMMAND ",
		    fourdos_name[8] = "4DOS    ",
		    ndos_name[8]    = "NDOS    ";
  switch (name[0])
    {
    case 'C':
      if (memcmp (name, command_name, 8) == 0)
	break;
      return false;
    case '4':
      if (memcmp (name, fourdos_name, 8) == 0)
	break;
      return false;
    case 'N':
      if (memcmp (name, ndos_name, 8) == 0)
	break;
      return false;
    default:
      return false;
    }

  return memcmp (ext, "COM", 3) == 0;
}

static bool
is_opt_c (const char *arg)
{
  /*
   * Say whether ARG is a command line switch that introduces a command to
   * run for a command.com-like interpreter.  Such a switch will normally
   * take a form like `/c' or `/C' (but may be `-c', depending on the kernel
   * "switchar" setting...).
   */
  char swchr = (char) _getswitchar ();
  return arg[0] != 0 && arg[0] == swchr
	 && (arg[1] == 'c' || arg[1] == 'C') && ! arg[2];
}

static bool
is_unix_shell (const char name[8], const char ext[3])
{
  static const char sh_name[8]		= "SH      ",
		    login_sh_name[8]	= "-SH     ",
		    sh16_name[8]	= "SH16    ",
		    sh32_name[8]	= "SH32    ",
		    tcsh_name[8]	= "TCSH    ",
		    login_tcsh_name[8]	= "-TCSH   ",
		    bash_name[8]	= "BASH    ",
		    login_bash_name[8]	= "-BASH   ";
  switch (name[2])
    {
    case ' ':
      if (memcmp (name, sh_name, 8) == 0)
	break;
      return false;
    case 'H':
      if (memcmp (name, login_sh_name, 8) == 0)
	break;
      return false;
    case '1':
      if (memcmp (name, sh16_name, 8) == 0)
	break;
      return false;
    case '3':
      if (memcmp (name, sh32_name, 8) == 0)
	break;
      return false;
    case 'S':
      if (memcmp (name, tcsh_name, 8) == 0
	  || memcmp (name, bash_name, 8) == 0)
	break;
      return false;
    case 'C':
      if (memcmp (name, login_tcsh_name, 8) == 0)
	break;
      return false;
    case 'A':
      if (memcmp (name, login_bash_name, 8) == 0)
	break;
      return false;
    default:
      return false;
    }

  return memcmp (ext, "EXE", 3) == 0;
}

static int
make_fcbs (struct _fcb *p_fcb1, struct _fcb *p_fcb2, const char *args,
	   _dos_dbcs_lead_table_t dbcs)
{
  const char *p = args;
  char c;
  struct _makefcb_t res;
  flags_t flags;

#define last_c_lead_p	flags.f7
#define done_arg1	flags.f0

  /*
   * The following implementation is my understanding of how MS-DOS 5.00's
   * command.com parses command line arguments into FCBs...
   *
   * Parse the first file name argument to the first default FCB.
   */
  res = _makefcb (p, p_fcb1, 1);
  p = res._tail;
  if (! p)
    {
      /* Something bad happened.  Assume we ran out of memory. */
      errno = ENOMEM;
      return -1;
    }

  /*
   * If there are any remaining file name characters immediately after any
   * first file name, skip over them.  Treat multi-byte characters as file
   * name characters.
   *
   * The _Microsoft MS-DOS Programmer's Reference: Including Version 5_
   * says (https://archive.org/details/isbn_9781556153297, p. 255):
   *
   *	``The following are valid filename separators:
   *		:  .  ;  ,  =  +
   *	  Once Parse Filename begins to convert a filename, it continues
   *	  to read characters from the string until it encounters a white-
   *	  -space character, a filename separator, a control character (ASCII
   *	  01h through 1Fh), or one of the following characters:
   *		/  "  [  ]  <  >  |
   *	''
   *
   * TODO: MS-DOS 5.0/V also treats the Shift-JIS "ideographic space" (0x81
   * 0x40) as whitespace, & not as part of a file name.  Should this routine
   * do the same?  And, should one generalize this treatment to multi-byte
   * code pages other than Shift-JIS?
   */
  do
    {
      c = *p++;
      switch (c)
	{
	case 0x00:  case 0x01:  case 0x02:  case 0x03:
	case 0x04:  case 0x05:  case 0x06:  case 0x07:
	case 0x08:  case 0x09:  case 0x0a:  case 0x0b:
	case 0x0c:  case 0x0d:  case 0x0e:  case 0x0f:
	case 0x10:  case 0x11:  case 0x12:  case 0x13:
	case 0x14:  case 0x15:  case 0x16:  case 0x17:
	case 0x18:  case 0x19:  case 0x1a:  case 0x1b:
	case 0x1c:  case 0x1d:  case 0x1e:  case 0x1f:
	  done_arg1 = true;
	  break;

	case ':':
	case '.':
	case ';':
	case ',':
	case '=':
	case '+':
	case ' ':
	case '/':
	case '"':
	case '[':
	case ']':
	case '<':
	case '>':
	case '|':
	  if (last_c_lead_p)
	    last_c_lead_p = false;
	  else
	    done_arg1 = true;
	  break;

	default:
	  if (last_c_lead_p)
	    last_c_lead_p = false;
	  else if (__libi86_msdos_dbcs_lead_p (c, dbcs))
	    last_c_lead_p = true;
	}
    }
  while (! done_arg1);
  --p;

  /* Parse the second file name argument to the second default FCB. */
  res = _makefcb (p, p_fcb2, 1);
  if (! res._tail)
    {
      errno = ENOMEM;
      return -1;
    }

  return 0;

#undef last_c_lead_p
#undef done_arg1
}

static int
make_env_blk (__libi86_segment_t *p_envb, char **p_envb_c,
	      const char * const *envp)
{
  const size_t PARA_SZ = 0x10;
  const char *p, * const *pp;
  size_t env_sz;
  __libi86_segment_t envb;
  unsigned envb_u;
  char *envb_c = NULL;
  __libi86_fpc_t fp;

  /*
   * Size up the environment variables, & allocate memory for the
   * environment block.  If we cannot obtain enough memory the C standard
   * way, or if we are in DOS extender mode, then ask MS-DOS (or the DOS
   * extender) for memory.  Copy out the environment.
   */
  env_sz = 1;
  pp = envp;
  while ((p = *pp++) != NULL)
    env_sz += strlen (p) + 1;

#if _LIBI86_ISO_C_VISIBLE >= 2011 && ! defined __IA16_FEATURE_DOSX
  envb_c = aligned_alloc (PARA_SZ, env_sz);
  if (envb_c)
    envb = FP_SEG (_CV_FP (envb_c)) + FP_OFF (_CV_FP (envb_c)) / PARA_SZ;
  else
#endif  /* >= C11 && ! __IA16_FEATURE_DOSX */
  if (_dos_allocmem ((env_sz - 1) / PARA_SZ + 1, &envb_u) != 0)
    {
      errno = ENOMEM;
      return -1;
    }
  envb = envb_u;

  pp = envp;
  fp = MK_FP (envb, 0);
  while ((p = *pp++) != NULL)
    {
      size_t sz = strlen (p) + 1;
      fp = _fmempcpy (fp, _CV_FP (p), sz);
    }
  __libi86_pokefpb (fp, 0);

  *p_envb = envb;
  *p_envb_c = envb_c;
  return 0;
}

static void
free_env_blk (__libi86_segment_t envb, char *envb_c)
{
  if (envb_c)
    free (envb_c);
  else
    _dos_freemem (envb);
}

static __libi86_pid_t
direct_spawn (int mode, unsigned char subfunc,
	      const char *path, const char name[8],
	      const char ext[3], const char * const *argv,
	      const char * const *envp, _dos_dbcs_lead_table_t dbcs)
{
  flags_t flags;
  const char *p, * const *pp;
  char argb[128], *q = argb + 1, c;
  struct _fcb fcb1, fcb2;
  __libi86_segment_t envb;
  char *envb_c = NULL;
  union _dosspawn_t spawn;
  unsigned res;

#define last_c_lead_p	flags.f7
#define dos_shell	flags.f6
#define opt_c_special	flags.f2
#define unix_shell	flags.f1
#define outside_quote	flags.f0

  dos_shell = is_dos_shell (name, ext);
  opt_c_special = dos_shell && (mode & _P_OPT_C_SPECIAL) != 0
		  && argv[1] && argv[2] && ! argv[3]
		  && is_opt_c (argv[1]);
  outside_quote = true;
  unix_shell = ! dos_shell && is_unix_shell (name, ext);

  /*
   * Decide how we should lay out the command line arguments.  Then lay them
   * out.
   */
#define COPY(chrs, len) \
	do \
	  { \
	    size_t _len = (len); \
	    if (_len > sizeof argb - 1 \
		|| q - argb > sizeof argb - 1 - _len) \
	      { \
		errno = E2BIG; \
		return -1; \
	      } \
	    memcpy (q, (chrs), _len); \
	    q += _len; \
	  } \
	while (0)
#define COPY1(chr) \
	do \
	  { \
	    if (q - argb == sizeof argb - 1) \
	      { \
		errno = E2BIG; \
		return -1; \
	      } \
	    *q++ = (chr); \
	  } \
	while (0)
#define COPY2(chr1, chr2) \
	do \
	  { \
	    if (q - argb == sizeof argb - 2) \
	      { \
		errno = E2BIG; \
		return -1; \
	      } \
	    *q++ = (chr1); \
	    *q++ = (chr2); \
	  } \
	while (0)
#define COPY3(chr1, chr2, chr3) \
	do \
	  { \
	    if (q - argb == sizeof argb - 3) \
	      { \
		errno = E2BIG; \
		return -1; \
	      } \
	    *q++ = (chr1); \
	    *q++ = (chr2); \
	    *q++ = (chr3); \
	  } \
	while (0)

  pp = argv + 1;
  while ((p = *pp++) != NULL)
    {
      COPY1 (' ');
      if (opt_c_special)
	COPY (p, strlen (p));
      else if (! dos_shell)
	{
	  if (*p != 0 && ! strpbrk (p, " \t"))
	    COPY (p, strlen (p));
	  else
	    {
	      last_c_lead_p = false;
	      COPY1 ('"');
	      while ((c = *p++) != 0)
		{
		  if (last_c_lead_p)
		    last_c_lead_p = false;
		  else
		    {
		      switch (c)
			{
			case '"':
			  COPY1 ('\\');
			  break;
			case '\\':
			  if (unix_shell)
			    COPY1 ('\\');
			  break;
			default:
			  if (__libi86_msdos_dbcs_lead_p (c, dbcs))
			    last_c_lead_p = true;
			}
		    }
		  COPY1 (c);
		}
	      COPY1 ('"');
	    }
	}
      else  /* DOS shell */
      if (*p != 0 && ! strpbrk (p, " \t<>|'\"%"))
	COPY (p, strlen (p));
      else
	{
	  last_c_lead_p = false;
	  COPY1 ('"');
	  outside_quote = ! outside_quote;
	  while ((c = *p++) != 0)
	    {
	      if (last_c_lead_p)
		{
		  last_c_lead_p = false;
		  COPY1 (c);
		}
	      else
		{
		  switch (c)
		    {
		    case '"':
		      COPY2 ('\\', '"');
		      outside_quote = ! outside_quote;
		      break;
		    case '%':
		      COPY2 ('%', '%');
		      break;
		    case '<':
		    case '>':
		    case '|':
		      if (outside_quote)
			COPY3 ('"', c, '"');
		      else
			COPY1 (c);
		      break;
		    default:
		      if (__libi86_msdos_dbcs_lead_p (c, dbcs))
			last_c_lead_p = true;
		      COPY1 (c);
		    }
		}
	    }
	  COPY1 ('"');
	  outside_quote = ! outside_quote;
	}
    }

#undef COPY
#undef COPY1
#undef COPY2
#undef COPY3
#undef last_c_lead_p
#undef dos_shell
#undef opt_c_special
#undef unix_shell
#undef outside_quote

  *q = '\r';
  argb[0] = q - argb - 1;

  if (make_fcbs (&fcb1, &fcb2, argb + 1, dbcs) != 0)
    return -1;

  if (make_env_blk (&envb, &envb_c, envp) != 0)
    return -1;

  memset (&spawn, 0, sizeof spawn);
  spawn._proc_run._env_seg = envb;
  spawn._proc_run._argv = _CV_FP (argb);
  spawn._proc_run._fcb1 = _CV_FP (&fcb1);
  spawn._proc_run._fcb2 = _CV_FP (&fcb2);
  res = _dos_spawn (subfunc, path, &spawn);

  free_env_blk (envb, envb_c);

  if (res != 0)
    return -1;

  _dos_wait (&res);
  return (__libi86_pid_t) res;
}

static const char *
find_raw_base_name (const char *path, _dos_dbcs_lead_table_t dbcs)
{
  const char *base, *p;
  char c;
  bool last_c_lead_p = false;

  p = base = path;
  while ((c = *p++) != 0)
    {
      if (last_c_lead_p)
	last_c_lead_p = false;
      else if (__libi86_msdos_dbcs_lead_p (c, dbcs))
	last_c_lead_p = true;
      else if (__libi86_msdos_path_sep_p (c) || c == ':')
	base = p;
    }

  return base;
}

static char *
find_raw_ext (const char *raw_base, _dos_dbcs_lead_table_t dbcs)
{
  char *ext = NULL, c;
  const char *p = raw_base;
  bool last_c_lead_p = false;

  while ((c = *p++) != 0)
    {
      if (last_c_lead_p)
	last_c_lead_p = false;
      else if (__libi86_msdos_dbcs_lead_p (c, dbcs))
	last_c_lead_p = true;
      else if (c == '.')
	ext = (char *) p - 1;
    }

  return ext;
}

static __libi86_pid_t
comspec_spawn (int mode, unsigned char subfunc,
	       const char *path, const char name[8], const char ext[3],
	       const char * const *argv, const char * const *envp,
	       _dos_dbcs_lead_table_t dbcs)
{
  const char *comspec = NULL, *p, * const *pp, **new_argv, **qq;
  struct _fcb comspec_fcb;
  size_t n_args = 0;
  char unslashed_path[_MAX_PATH], opt_c[] = "/c", *q, c;
  pid_t res;
  bool last_c_lead_p = false;

  /*
   * The DJGPP sources point out that command.com does not handle program
   * names with forward slashes.  Replace these slashes with backslashes.
   *
   * Also look out for "backslashes" that are really part of multi-byte
   * characters.
   */
  if (strchr (path, '/'))
    {
      size_t path_len = strlen (path);
      if (path_len >= _MAX_PATH)
	{
#ifdef _LIBI86_INTERNAL_HAVE_ENAMETOOLONG
	  errno = ENAMETOOLONG;
#else
	  errno = E2BIG;
#endif
	  return -1;
	}

      p = path;
      q = unslashed_path;
      do
	{
	  c = *p++;
	  if (last_c_lead_p)
	    last_c_lead_p = false;
	  else
	    switch (c)
	      {
	      case 0:
	        break;
	      case '/':
		c = '\\';
	        break;
	      default:
		if (__libi86_msdos_dbcs_lead_p (c, dbcs))
		  last_c_lead_p = true;
	      }
	  *q++ = c;
	}
      while (c != 0);

      path = unslashed_path;
    }

  comspec = __libi86_comspec (envp);
  if (! _parsfnm (find_raw_base_name (comspec, dbcs), &comspec_fcb, 0))
    {
      errno = EINVAL;
      return -1;
    }

  pp = argv + 1;
  while (*pp++ != NULL)
    ++n_args;

  new_argv = malloc ((n_args + 4) * sizeof (const char *));
  if (! new_argv)
    return -1;

  opt_c[0] = (char) _getswitchar ();
  new_argv[0] = comspec;
  new_argv[1] = opt_c;
  new_argv[2] = path;
  pp = argv + 1;
  qq = new_argv + 3;
  do
    {
      p = *pp++;
      *qq++ = p;
    }
  while (p);

  res = direct_spawn (mode & ~_P_OPT_C_SPECIAL, subfunc,
		      comspec, comspec_fcb._fcb_name, comspec_fcb._fcb_ext,
		      new_argv, envp, dbcs);
  free (new_argv);
  return res;
}

static __libi86_pid_t
error_spawn (int mode, unsigned char subfunc,
	     const char *path, const char name[8], const char ext[3],
	     const char * const *argv, const char * const *envp,
	     _dos_dbcs_lead_table_t dbcs)
{
  errno = EACCES;
  return -1;
}

static spawner_t
find_spawner (const char ext[3], bool restrict_ext, bool interp)
{
  spawner_t spawner = error_spawn;

  if (! restrict_ext)
    spawner = direct_spawn;

  switch (ext[1])
    {
    case 'A':
      if (ext[0] == 'B' && ext[2] == 'T')  /* BAT */
	spawner = comspec_spawn;
      break;

    case 'O':
      if (ext[0] == 'C' && ext[2] == 'M')  /* COM */
	spawner = direct_spawn;
      break;

    case 'T':
      if (ext[0] == 'B' && ext[2] == 'M')  /* BTM */
	spawner = comspec_spawn;
      break;

    case 'X':
      if (ext[0] == 'E' && ext[2] == 'E')  /* EXE */
	spawner = direct_spawn;
    }

  if (interp && spawner == comspec_spawn)
    spawner = direct_spawn;

  return spawner;
}

__libi86_pid_t
_spawnve (int mode, const char *path, const char * const *argv,
	  const char * const *envp)
{
  _dos_dbcs_lead_table_t dbcs;
  spawner_t spawner;
  struct _fcb fcb;
  char path_with_ext[_MAX_PATH];
  unsigned char subfunc;
  const char *raw_base;
  unsigned attrs;
  bool restrict_ext, interp;

  if (! path || ! path[0] || ! argv || ! argv[0])
    {
      errno = EINVAL;
      return -1;
    }

  switch (mode & ~_P_SPVE_FLAGS)
    {
    case _P_WAIT:
      subfunc = 0;
      break;

    default:
      errno = EINVAL;
      return -1;
    }

  dbcs = _dos_get_dbcs_lead_table ();

  /*
   * Try to locate the executable to run.
   *
   * If the file path has no extension, first try to add a .COM, .EXE, .BAT,
   * or .BTM extension, _then_ if none of these can be found --- & if
   * _P_RESTRICT_EXT is not specified --- we try to look for the extension-
   * -less file.
   *
   * (This differs from what Open Watcom does, but is more similar to
   * DJGPP's behaviour, & is arguably safer.)
   */
  restrict_ext = ((mode & _P_RESTRICT_EXT) != 0);
  interp = ((mode & _P_INTERP) != 0);
  raw_base = find_raw_base_name (path, dbcs);

  if (interp || find_raw_ext (raw_base, dbcs))
    {
      if (_dos_getfileattr (path, &attrs) != 0)
	return -1;
    }
  else
    {
      char *raw_ext;
      size_t path_len = strlen (path);

      if (path_len > _MAX_PATH - 5)
	{
#ifdef _LIBI86_INTERNAL_HAVE_ENAMETOOLONG
	  errno = ENAMETOOLONG;
#else
	  errno = E2BIG;
#endif
	  return -1;
	}

      memcpy (path_with_ext, path, path_len);
      path = path_with_ext;
      raw_ext = path_with_ext + path_len;

      strcpy (raw_ext, ".COM");
      if (_dos_getfileattr (path, &attrs) != 0)
	{
	  strcpy (raw_ext + 1, "EXE");
	  if (_dos_getfileattr (path, &attrs) != 0)
	    {
	      strcpy (raw_ext + 1, "BAT");
	      if (_dos_getfileattr (path, &attrs) != 0)
		{
		  strcpy (raw_ext + 1, "BTM");
		  if (_dos_getfileattr (path, &attrs) != 0)
		    {
		      if (restrict_ext)
			return -1;

		      *raw_ext = 0;
		      if (_dos_getfileattr (path, &attrs) != 0)
			return -1;
		    }
		}
	    }
	}

      raw_base = find_raw_base_name (path, dbcs);
    }

  /*
   * Parsing the base file name (i.e. sans directory path) to an FCB is a
   * simple (if not quick) way to "canonicalize" the base name, & to allow
   * us to compare the name with known program names.  So parse it.
   */
  if (! _parsfnm (raw_base, &fcb, 0))
    {
      errno = EINVAL;
      return -1;
    }

  if (! envp)
    envp = (const char **) environ;

  /* Decide how we need to run the executable. */
  spawner = find_spawner (fcb._fcb_ext, restrict_ext, interp);

  /* Then run it. */
  return spawner (mode, subfunc, path, fcb._fcb_name, fcb._fcb_ext,
		  argv, envp, dbcs);
}

#ifdef __GNUC__
_LIBI86_WEAK_ALIAS (_spawnve) __libi86_pid_t
spawnve (int, const char *, const char * const *, const char * const *);
#endif
