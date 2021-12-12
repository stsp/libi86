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

#ifndef _LIBI86_LIBI86_INTERNAL_DOS_H_
#define _LIBI86_LIBI86_INTERNAL_DOS_H_

#ifndef _LIBI86_COMPILING_
# error "<libi86/internal/dos.h> should only be used when compiling libi86!"
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <i86.h>
#include <libi86/stdlib.h>
#include <libi86/internal/cdefs.h>
#include <libi86/internal/dos-dbcs.h>
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include <dpmi.h>
#else
# include <errno.h>
#endif

_LIBI86_BEGIN_EXTERN_C

typedef struct
{
  uint8_t drive, name[8], ext[3], unused[25];
} __libi86_msdos_fcb_t;

#ifdef __GNUC__
extern __attribute__ ((regparmcall)) unsigned
#else
extern unsigned
#endif
__libi86_ret_really_set_errno (unsigned);
#if defined __GNUC__ && defined __IA16_FEATURE_PROTECTED_MODE
extern struct find_t __far *__libi86_dpmi_set_dta (void);
extern dpmi_dos_block __libi86_dpmi_low_dup_str (const char *);
extern void __libi86_dpmi_low_free_str (dpmi_dos_block);
extern unsigned __libi86_dpmi_pm_to_rm_buf (const void __far *, size_t, bool,
					    _LIBI86_SEG_SELECTOR *, size_t *);
#endif

_LIBI86_STATIC_INLINE bool
__libi86_msdos_drive_letter_p (char c)
{
  switch (c)
    {
    case 'A':  case 'B':  case 'C':  case 'D':  case 'E':  case 'F':  case 'G':
    case 'H':  case 'I':  case 'J':  case 'K':  case 'L':  case 'M':  case 'N':
    case 'O':  case 'P':  case 'Q':  case 'R':  case 'S':  case 'T':  case 'U':
    case 'V':  case 'W':  case 'X':  case 'Y':  case 'Z':
    case 'a':  case 'b':  case 'c':  case 'd':  case 'e':  case 'f':  case 'g':
    case 'h':  case 'i':  case 'j':  case 'k':  case 'l':  case 'm':  case 'n':
    case 'o':  case 'p':  case 'q':  case 'r':  case 's':  case 't':  case 'u':
    case 'v':  case 'w':  case 'x':  case 'y':  case 'z':
      return true;
    default:
      return false;
    }
}

_LIBI86_STATIC_INLINE bool
__libi86_msdos_path_sep_p (char c)
{
  switch (c)
    {
    case '/':
    case '\\':
      return true;
    default:
      return false;
    }
}

_LIBI86_STATIC_INLINE bool
__libi86_msdos_has_drive_spec_p (const char *path)
{
  char c0 = path[0];
  return c0 != 0 && path[1] == ':' && __libi86_msdos_drive_letter_p (c0);
}

#ifdef __GNUC__
_LIBI86_STATIC_INLINE void
__libi86_msdos_set_dta (void __far *new_dta)
{
  unsigned xx1, xx2;
  __asm volatile ("int $0x21"
		  : "=a" (xx1), "=d" (xx2)
		  : "Rah" ((unsigned char) 0x1a),
		    "Rds" (FP_SEG (new_dta)), "1" (FP_OFF (new_dta))
		  : "cc", "bx", "cx", "memory");
}
#else  /* ! __GNUC__ */
extern void __libi86_msdos_set_dta (void *new_dta);
extern unsigned __libi86_msdos_do_findfirst (const char *path, unsigned attr);
extern unsigned __libi86_msdos_do_findnext (void);
extern unsigned __libi86_msdos_do_open (const char *path, unsigned mode,
					int *handle);
#endif  /* ! __GNUC__ */

#if defined __GNUC__ && ! defined __IA16_FEATURE_PROTECTED_MODE
_LIBI86_STATIC_INLINE int
__libi86_msdos_do_getdcwd (char buf[_MAX_PATH - 3], unsigned char drive)
{
  int err, carry, xx1, xx2;
  __asm volatile ("int $0x21; sbbw %1, %1"
		  : "=a" (err), "=r" (carry)
		  : "Rah" ((unsigned char) 0x47), "Rdl" (drive),
		    "Rds" (FP_SEG (buf)), "S" (FP_OFF (buf))
		  : "cc", "memory");
  if (carry)
    errno = err;
  return carry;
}
#else  /* ! __GNUC__ || __IA16_FEATURE_PROTECTED_MODE */
extern int __libi86_msdos_do_getdcwd (char buf[_MAX_PATH - 3],
				      unsigned char drive);
#endif  /* ! __GNUC__ || __IA16_FEATURE_PROTECTED_MODE */

extern unsigned __libi86_msdos_do_truename (const char *path, char *out_path);
extern const char *__libi86_msdos_parse_to_fcb (const char *name,
						__libi86_msdos_fcb_t *fcb);

/*
 * Iterate through the directories in %PATH%.  At each iteration, set
 * PATHNAME to point to a file path comprising the directory path & the file
 * name component NAME.  DBCS should point to a DBCS lead byte table.  ITR
 * should be a __libi86_msdos_path_itr_t object.
 *
 * If an error occurred while trying to set PATHNAME, the loop will
 * terminate prematurely & set errno to an error code (e.g. ENAMETOOLONG). 
 * Otherwise, after the loop terminates, errno will be set to (!) ENOENT. 
 * This caters to the common case where we need to find a particular file
 * along the %PATH% search path.
 */
#define _LIBI86_FOR_EACH_PATHED_PATHNAME(pathname, name, dbcs, itr) \
	for ((pathname) = __libi86_msdos_pathed_first ((name), (dbcs), \
						       &(itr)); \
	     (pathname); \
	     (pathname) = __libi86_msdos_pathed_next (&(itr)))

typedef struct
{
  char __pathname_[_MAX_PATH];
  const char *__name_, *__next_path_;
  _dos_dbcs_lead_table_t __dbcs_;
} __libi86_msdos_path_itr_t;

extern char *__libi86_msdos_pathed_first (const char *,
					  _dos_dbcs_lead_table_t,
					  __libi86_msdos_path_itr_t *);
extern char *__libi86_msdos_pathed_next (__libi86_msdos_path_itr_t *);

#endif
