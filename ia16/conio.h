/*
 * Copyright (c) 2018--2020 TK Chia
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

#ifndef _LIBI86_CONIO_H_
#define _LIBI86_CONIO_H_

#include <libi86/internal/cdefs.h>
#include <libi86/internal/portio.h>

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
extern int _LIBI86_REDIRECT (getch, (void), _getch);
# ifdef _BORLANDC_SOURCE
extern int _LIBI86_BC_REDIRECT (getche, (void));
# else
extern int _LIBI86_REDIRECT (getche, (void), _getche);
# endif
extern int _LIBI86_REDIRECT (kbhit, (void), _kbhit);
extern int _LIBI86_REDIRECT (ungetch, (int), _ungetch);
#endif

/*
 * These functions are implemented differently in default Watcom mode & in
 * Borand compatibility mode (!).  If _BORLANDC_SOURCE is defined, cputs (.)
 * for example is redirected to __libi86_bc_cputs (.).
 */
extern char *_LIBI86_BC_REDIRECT (cgets, (char *));
extern int _LIBI86_BC_REDIRECT (cprintf, (const char *, ...))
	   __attribute__ ((__format__ (__printf__, 1, 2)));
extern int _LIBI86_BC_REDIRECT (cputs, (const char *));
extern int _LIBI86_BC_REDIRECT (cscanf, (const char *, ...))
	   __attribute__ ((__format__ (__scanf__, 1, 2)))
	   _LIBI86_DEPRECATED ("libi86 cscanf is hard to control, and works "
			       "differently from Open Watcom cscanf");
extern int _LIBI86_BC_REDIRECT (putch, (int));
extern int _LIBI86_BC_REDIRECT (vcprintf, (const char *, _LIBI86_VA_LIST));
extern int _LIBI86_BC_REDIRECT (vcscanf, (const char *, _LIBI86_VA_LIST))
	   _LIBI86_DEPRECATED ("libi86 vcscanf is hard to control, and works "
			       "differently from Open Watcom vcscanf");

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
# ifdef _BORLANDC_SOURCE
_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, inportb, unsigned, __libi86_inp)
_LIBI86_REDIRECT_AND_INLINE_1 (unsigned, inportw, unsigned, __libi86_inpw)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, outportb, unsigned, unsigned,
			       __libi86_outp)
_LIBI86_REDIRECT_AND_INLINE_2 (unsigned, outportw, unsigned, unsigned,
			       __libi86_outpw)
# endif /* _BORLANDC_SOURCE */
#endif /* _LIBI86_COMPILING */

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

extern void clrscr (void);
extern void gotoxy (int __x, int __y);
extern void textmode (int);
extern int wherex (void);
extern int wherey (void);
extern void window (int __left, int __top, int __right, int __bottom);
#endif /* _BORLANDC_SOURCE */

_LIBI86_END_EXTERN_C

#endif
