/*
 * Copyright (c) 2021--2022 TK Chia
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

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <i86.h>
#include <libi86/stdlib.h>
#include <libi86/internal/cdefs.h>
#include <libi86/internal/dos-dbcs.h>

_LIBI86_BEGIN_EXTERN_C

#ifdef __GNUC__
extern __attribute__ ((regparmcall)) unsigned
#else
extern unsigned
#endif
__libi86_ret_really_set_errno (unsigned);

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

_LIBI86_STATIC_INLINE int
__libi86_msdos_do_getdcwd (char buf[_MAX_PATH - 3], unsigned char drive)
{
  int err, carry;
  __asm volatile ("int $0x21; sbbw %1, %1"
		  : "=a" (err), "=dr" (carry)
		  : "Rah" ((unsigned char) 0x47), "Rdl" (drive),
		    "Rds" (FP_SEG (buf)), "S" (FP_OFF (buf))
		  : "cc", "bx", "cx", "memory");
  if (carry)
    errno = err;
  return carry;
}
#else  /* ! __GNUC__ */
extern void __libi86_msdos_set_dta (void *new_dta);
extern unsigned __libi86_msdos_do_findfirst (const char *path, unsigned attr);
extern unsigned __libi86_msdos_do_findnext (void);
extern unsigned __libi86_msdos_do_open (const char *path, unsigned mode,
					int *handle);
extern int __libi86_msdos_do_getdcwd (char buf[_MAX_PATH - 3],
				      unsigned char drive);
#endif  /* ! __GNUC__ */

extern long __libi86_ret_einval (void);
#ifdef __GNUC__
extern __attribute__ ((regparmcall)) unsigned
       __libi86_ret_really_set_errno (unsigned);
#else
extern unsigned __libi86_ret_really_set_errno (unsigned);
#endif
extern unsigned __libi86_msdos_do_truename (const char *path, char *out_path);

#ifdef __GNUC__
typedef struct
{
  unsigned ax;
  int carry;
} __libi86_bdos_res_t;

_LIBI86_STATIC_INLINE unsigned char
__libi86_any8 (void)
{
  unsigned char x;
  __asm volatile ("" : "=r" (x));
  return x;
}

_LIBI86_STATIC_INLINE unsigned
__libi86_any16 (void)
{
  unsigned x;
  __asm volatile ("" : "=r" (x));
  return x;
}

_LIBI86_STATIC_INLINE __libi86_bdos_res_t
__libi86_bdos_dsdxsz_al_cx (unsigned char dos_func, const char *dsdx,
			    unsigned char al, unsigned cx)
{
  __libi86_bdos_res_t res;
  unsigned xx1;
  __asm volatile ("int $0x21; sbbw %1, %1"
		  : "=a" (res.ax), "=d" (res.carry), "=c" (xx1)
		  : "Rah" (dos_func), "Ral" (al), "c" (cx),
		    "Rds" (FP_SEG (dsdx)), "1" (FP_OFF (dsdx))
		  : "cc", "bx", "memory");
  if (res.carry)
    errno = res.ax;
  return res;
}

_LIBI86_STATIC_INLINE __libi86_bdos_res_t
__libi86_bdos_dsdxsz_al (unsigned char dos_func, const char *dsdx,
			 unsigned char al)
{
  __libi86_bdos_res_t res;
  __asm volatile ("int $0x21; sbbw %1, %1"
		  : "=a" (res.ax), "=d" (res.carry)
		  : "Rah" (dos_func), "Ral" (al),
		    "Rds" (FP_SEG (dsdx)), "1" (FP_OFF (dsdx))
		  : "cc", "bx", "cx", "memory");
  if (res.carry)
    errno = res.ax;
  return res;
}

_LIBI86_STATIC_INLINE __libi86_bdos_res_t
__libi86_bdos_dsdxsz (unsigned char dos_func, const char *dsdx)
{
  __libi86_bdos_res_t res;
  __asm volatile ("int $0x21; sbbw %1, %1"
		  : "=a" (res.ax), "=d" (res.carry)
		  : "Rah" (dos_func),
		    "Rds" (FP_SEG (dsdx)), "1" (FP_OFF (dsdx))
		  : "cc", "bx", "cx", "memory");
  if (res.carry)
    errno = res.ax;
  return res;
}
#endif  /* __GNUC__ */

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
