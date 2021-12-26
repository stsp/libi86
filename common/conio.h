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

#ifndef _LIBI86_CONIO_H_
#define _LIBI86_CONIO_H_

#include <libi86/internal/cdefs.h>
#include <libi86/internal/portio.h>
#include <libi86/internal/bc-portio.h>

_LIBI86_BEGIN_EXTERN_C

/*
 * To avoid conflict with the getch () and ungetch (.) routines in curses
 * library implementations, I define _getch () and _ungetch (.) in the
 * object files, and here declare getch () and ungetch (.) to be aliases for
 * the underscored names.
 *
 * For uniformity, I treat getche () and kbhit () the same way.  getche ()
 * is a bit of a special case, since it needs to be redirected differently
 * in Borland compatibility mode; see below.
 *
 * On the assembly source front, I also define weak symbols getch, getche,
 * etc. --- however, these are not actually used (by <conio.h>), and they
 * can be overridden.  Defining these non-underscored symbols allows
 * make-config.sh to spot them and proclaim them as being implemented.  :-)
 *
 * (Even with all this, it is probably still a bad idea to use both
 * <conio.h> and <curses.h> in the same C module.)
 */
extern int _getch (void);
extern int _getche (void);
extern int _kbhit (void);
extern int _ungetch (int);
#ifndef _LIBI86_COMPILING_
_LIBI86_REDIRECT_0 (int, getch, _getch)
# ifdef _BORLANDC_SOURCE
_LIBI86_BC_REDIRECT_0 (int, getche)
# else
_LIBI86_REDIRECT_0 (int, getche, _getche)
# endif
_LIBI86_REDIRECT_0 (int, kbhit, _kbhit)
_LIBI86_REDIRECT_1 (int, ungetch, int, _ungetch)
#endif

/*
 * These functions are implemented differently in default Watcom mode & in
 * Borand compatibility mode (!).  If _BORLANDC_SOURCE is defined, cputs (.)
 * for example is redirected to __libi86_bc_cputs (.).
 */
_LIBI86_BC_REDIRECT_1 (char *, cgets, char *)
_LIBI86_BC_REDIRECT_1 (int, cputs, const char *)
_LIBI86_BC_REDIRECT_1 (int, putch, int)
_LIBI86_BC_REDIRECT_2 (int, vcprintf, const char *, __libi86_va_list_t)
#ifdef __GNUC__
extern int _LIBI86_BC_REDIRECT_X (cprintf, (const char *, ...))
	   __attribute__ ((__format__ (__printf__, 1, 2)));
extern int _LIBI86_BC_REDIRECT_X (cscanf, (const char *, ...))
	   __attribute__ ((__format__ (__scanf__, 1, 2)))
	   _LIBI86_DEPRECATED ("libi86 cscanf is hard to control, and works "
			       "differently from Open Watcom cscanf");
extern int _LIBI86_BC_REDIRECT_X (vcscanf, (const char *, __libi86_va_list_t))
	   _LIBI86_DEPRECATED ("libi86 vcscanf is hard to control, and works "
			       "differently from Open Watcom vcscanf");
#else  /* ! __GNUC__ */
_LIBI86_BC_REDIRECT_2 (int, vcscanf, const char *, __libi86_va_list_t)
# ifdef _BORLANDC_SOURCE
#   define cprintf	__libi86_bc_cprintf	/* FIXME */
#   define cscanf	__libi86_bc_cscanf
# endif  /* _BORLANDC_SOURCE */
extern int cprintf (const char *, ...);
extern int cscanf (const char *, ...);
#endif  /* ! __GNUC__ */

#ifndef _LIBI86_COMPILING_
_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, inp, unsigned, __libi86_inp)
_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, _inp, unsigned, __libi86_inp)
_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, inpw, unsigned, __libi86_inpw)
_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, _inpw, unsigned, __libi86_inpw)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, outp, unsigned, unsigned,
			       __libi86_outp)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, _outp, unsigned, unsigned,
			       __libi86_outp)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, outpw, unsigned, unsigned,
			       __libi86_outpw)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, _outpw, unsigned, unsigned,
			       __libi86_outpw)
#endif /* ! _LIBI86_COMPILING */

/* These functions & constants are from Borland C++, & are not in Watcom. */
#ifdef _BORLANDC_SOURCE
enum text_modes
{
  BW40 = 0x0000,
  C40 = 0x0001,
  BW80 = 0x0002,
  C80 = 0x0003,
  MONO = 0x0007,
  C4350 = 0x0040
  /*
   * Borland Turbo C++ 3.0 also defines LASTMODE = -1, but its precise
   * intended meaning is still unclear to me.  -- tkchia 20201009
   */
};

enum COLORS
{
  BLACK, BLUE, GREEN, CYAN,
  RED, MAGENTA, BROWN, LIGHTGRAY,
  DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN,
  LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
};

#define BLINK		0x80

struct text_info
{
  unsigned char winleft, wintop, winright, winbottom, attribute, normattr,
		currmode, screenheight, screenwidth, curx, cury;
};

extern void clreol (void);
extern void clrscr (void);
extern void delline (void);
extern char *getpass (const char *__prompt);
extern void gettextinfo (struct text_info *__r);
extern void gotoxy (int __x, int __y);
extern void highvideo (void);
extern void insline (void);
extern void lowvideo (void);
extern void normvideo (void);
extern void textattr (int __new_attr);
extern void textbackground (int __new_color);
extern void textcolor (int __new_color);
extern void textmode (int);
extern int wherex (void);
extern int wherey (void);
extern void window (int __left, int __top, int __right, int __bottom);
#endif /* _BORLANDC_SOURCE */

_LIBI86_END_EXTERN_C

#endif
