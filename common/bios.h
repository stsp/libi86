/*
 * Copyright (c) 2018--2021 TK Chia
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

#ifndef _LIBI86_BIOS_H_
#define _LIBI86_BIOS_H_

#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

struct diskinfo_t
{
  unsigned drive, head, track, sector, nsectors;
  __libi86_fpv_t buffer;
};

/* Not in Open Watcom.  */
union _joyinfo_t
{
  struct
    {
      unsigned __ax, __bx, __cx, __dx;
    } __raw;
  unsigned char trig;
  struct
    {
      unsigned x_a, y_a, x_b, y_b;
    } pos;
};

#ifdef __GNUC__
_LIBI86_ALT_INLINE unsigned short
_bios_equiplist (void)
{
  unsigned a;
  __asm volatile ("int {$}0x11" : "=a" (a));
  return a;
}

_LIBI86_ALT_INLINE unsigned short
_bios_memsize (void)
{
  unsigned a;
  __asm volatile ("int {$}0x12" : "=a" (a));
  return a;
}
#else  /* ! __GNUC__ */
extern unsigned short _bios_equiplist (void);
extern unsigned short _bios_memsize (void);
#endif  /* ! __GNUC__ */

extern unsigned short _bios_disk (unsigned __service,
				  struct diskinfo_t *__diskinfo);
extern unsigned short _bios_keybrd (unsigned __service);
extern unsigned short _bios_printer (unsigned __cmd, unsigned __port,
				     unsigned __data);
extern unsigned short _bios_serialcom (unsigned __cmd, unsigned __port,
				       unsigned __data);
/* Note:  The Open Watcom C Library Reference gives the prototype
	int _bios_timeofday (int service, long *__timeval);
   with different argument and return types.  I follow the actual Open Watcom
   <bios.h> prototype here.  */
extern unsigned short _bios_timeofday (unsigned __service, long *__timeval);
/* Not in Open Watcom.  */
extern unsigned short _bios_joystick (unsigned __service,
				      union _joyinfo_t *__joyinfo);

#define _DISK_RESET		0
#define _DISK_STATUS		1
#define _DISK_READ		2
#define _DISK_WRITE		3
#define _DISK_VERIFY		4
#define _DISK_FORMAT		5
/* Not in Open Watcom.  */
#define _DISK_DRIVEPARAMS	8

#define _COM_INIT		0
#define _COM_SEND		1
#define _COM_RECEIVE		2
#define _COM_STATUS		3

#define _COM_110		0x00u
#define _COM_150		0x20u
#define _COM_300		0x40u
#define _COM_600		0x60u
#define _COM_1200		0x80u
#define _COM_2400		0xa0u
#define _COM_4800		0xc0u
#define _COM_9600		0xe0u

#define _COM_NOPARITY		0x00u
#define _COM_ODDPARITY		0x08u
#define _COM_SPACEPARITY	0x10u
#define _COM_EVENPARITY		0x18u

#define _COM_STOP1		0x00u
#define _COM_STOP2		0x04u

#define _COM_CHR7		0x02u
#define _COM_CHR8		0x03u

#define _KEYBRD_READ		0x00u
#define _KEYBRD_READY		0x01u
#define _KEYBRD_SHIFTSTATUS	0x02u
#define _NKEYBRD_READ		0x10u
#define _NKEYBRD_READY		0x11u
#define _NKEYBRD_SHIFTSTATUS	0x12u

#define _PRINTER_WRITE		0
#define _PRINTER_INIT		1
#define _PRINTER_STATUS		2

#define _TIME_GETCLOCK		0x00u
#define _TIME_SETCLOCK		0x01u

/* Not in Open Watcom.  */
#define _JOY_READTRIG		0
#define _JOY_READPOS		1

#ifdef _BORLANDC_SOURCE
_LIBI86_REDIRECT_AND_INLINE_0 (unsigned short, biosequip, _bios_equiplist)
_LIBI86_REDIRECT_AND_INLINE_0 (unsigned short, biosmemory, _bios_memsize)
_LIBI86_REDIRECT_1 (unsigned short, bioskey, unsigned, _bios_keybrd)
#endif /* _BORLANDC_SOURCE */

_LIBI86_END_EXTERN_C

#endif
