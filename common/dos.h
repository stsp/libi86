/*
 * Copyright (c) 2018--2022 TK Chia
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

#ifndef _LIBI86_DOS_H_
#define _LIBI86_DOS_H_

#include <libi86/internal/cdefs.h>
#include <libi86/internal/bc-portio.h>

/* <dos.h> should contain everything that <i86.h> has (at least for the IA-16
   platform...).  */
#include <i86.h>

#define _A_NORMAL	0x00u
#define _A_RDONLY	0x01u
#define _A_HIDDEN	0x02u
#define _A_SYSTEM	0x04u
#define _A_VOLID	0x08u
#define _A_SUBDIR	0x10u
#define _A_ARCH		0x20u

#define O_NOINHERIT	0x80

_LIBI86_BEGIN_EXTERN_C

struct dosdate_t
{
  unsigned char day;
  unsigned char month;
  unsigned short year;
  unsigned char dayofweek;
};

struct dostime_t
{
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
  unsigned char hsecond;
};

struct diskfree_t
{
  unsigned total_clusters, avail_clusters, sectors_per_cluster,
	   bytes_per_sector;
};

struct
#ifdef __GNUC__
__attribute__ ((__packed__))
#endif
find_t
{
  char reserved[21];
  char attrib;
  unsigned short wr_time;
  unsigned short wr_date;
  unsigned long size;
  char name[13];
};

extern int bdos (int __dos_func, unsigned __dx, unsigned __al);
#ifdef _BORLANDC_SOURCE
_LIBI86_REDIRECT_3 (int, bdosptr, int, void *, unsigned, __libi86_bdosptr)
#endif
_LIBI86_BC_REDIRECT_2 (int, intdos, const union REGS *, union REGS *)
_LIBI86_BC_REDIRECT_3 (int, intdosx, const union REGS *, union REGS *,
				     struct SREGS *)
extern unsigned _dos_allocmem (unsigned __size, unsigned *__seg);
extern unsigned _dos_close (int __handle);
extern unsigned _dos_commit (int __handle);
extern unsigned _dos_creat (const char *__path, unsigned __attr,
			    int *__handle);
extern unsigned _dos_creatnew (const char *__path, unsigned __attr,
			       int *__handle);
extern unsigned _dos_findfirst (const char *__path, unsigned __attr,
				struct find_t *__buf);
extern unsigned _dos_findnext (struct find_t *__buf);
#ifndef __ACK
extern unsigned _dos_findclose (struct find_t *__buf);
#endif
extern unsigned _dos_freemem (unsigned __seg);
extern void _dos_getdate (struct dosdate_t *__date);
extern unsigned _dos_getdiskfree (unsigned __drive,
				  struct diskfree_t *__diskspace);
extern void _dos_getdrive (unsigned *__drive);
extern unsigned _dos_getfileattr (const char *__path, unsigned *__attributes);
extern unsigned _dos_getftime (int __handle,
			       unsigned *__date, unsigned *__time);
extern void _dos_gettime (struct dostime_t *__time);
extern void _dos_keep (unsigned __status, unsigned __size) _LIBI86_NORETURN;
extern unsigned _dos_open (const char *__path, unsigned __mode, int *__handle);
extern unsigned _dos_read (int __handle, __libi86_fpv_t __buf,
			   unsigned __count, unsigned *__bytes);
extern unsigned _dos_setblock (unsigned __size, unsigned __seg,
			       unsigned *__max_size);
extern unsigned _dos_setdate (const struct dosdate_t *__date);
extern void _dos_setdrive (unsigned __drive, unsigned *__total);
extern unsigned _dos_setfileattr (const char *__path, unsigned __attributes);
extern unsigned _dos_setftime (int __handle, unsigned __date, unsigned __time);
extern unsigned _dos_settime (const struct dostime_t *__time);
extern unsigned _dos_write (int __handle, __libi86_fpcv_t __buf,
			    unsigned __count, unsigned *__bytes);
/*
 * Note:  The Open Watcom C Library Reference has
 *	int _getdrive (void);
 * but the actual <dos.h> and <direct.h> files in Open Watcom both give an
 * unsigned return type.
 */
extern unsigned _getdrive (void);

#ifdef __INTERRUPT
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wstrict-prototypes"
typedef void __interrupt __far (*__libi86_isr_t) (/* ... */);
# pragma GCC diagnostic pop
extern __libi86_isr_t _dos_getvect (unsigned __intr_no);
extern void _dos_setvect (unsigned __intr_no, __libi86_isr_t __isr);
#else
typedef __libi86_fpcv_t __libi86_isr_t;
extern __libi86_isr_t _dos_getvect (unsigned __intr_no)
		      _LIBI86_WARNING ("_dos_getvect (.) not fully supported: "
				       "__interrupt unrecognized");
extern void _dos_setvect (unsigned __intr_no, __libi86_isr_t __isr)
	    _LIBI86_ERROR ("_dos_setvect (.) not supported: "
			   "__interrupt unrecognized");
#endif

_LIBI86_ALT_INLINE unsigned
_dos_findclose (struct find_t *__buf)
{
  return 0;
}

#ifdef _BORLANDC_SOURCE
_LIBI86_REDIRECT_AND_INLINE_2 (int, peek, unsigned, unsigned, __libi86_peek)
_LIBI86_REDIRECT_AND_INLINE_2 (char, peekb, unsigned, unsigned, __libi86_peekb)
_LIBI86_REDIRECT_AND_INLINE_VOID_3 (poke, unsigned, unsigned, int,
				    __libi86_poke)
_LIBI86_REDIRECT_AND_INLINE_VOID_3 (pokeb, unsigned, unsigned, char,
				    __libi86_pokeb)
/*
 * Note:  Watcom's _dos_keep (, ) has an `unsigned' parameter for the exit
 * code, but Borland's keep (, ) has an `unsigned char' parameter.
 */
# ifdef __GNUC__
extern void _LIBI86_BC_REDIRECT_X (keep, (unsigned char, unsigned))
	    _LIBI86_NORETURN;
# else
_LIBI86_REDIRECT_AND_INLINE_VOID_2 (keep, unsigned char, unsigned,
				    __libi86_bc_keep)
# endif
#endif  /* _BORLANDC_SOURCE */

/* For testing purposes. */
#ifdef __GNUC__
# if #system (msdos bdos call5)
#   ifdef __TINY__
_LIBI86_REDIRECT_3 (int, bdos, int, unsigned, unsigned, __libi86_call5_tiny)
#   else
_LIBI86_REDIRECT_3 (int, bdos, int, unsigned, unsigned, __libi86_call5)
#   endif
#   define _LIBI86_USE_BDOS_CALL5
# endif
#endif

_LIBI86_END_EXTERN_C

#endif  /* ! _LIBI86_DOS_H_ */
