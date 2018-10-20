/*
 * Copyright (c) 2018 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING3.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef _LIBI86_CONIO_H_
#define _LIBI86_CONIO_H_

#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

/*
 * To avoid conflict with the getch () and ungetch (.) routines in curses
 * library implementations, I only actually define _getch () and _ungetch
 * (.) in the object files, and here declare getch () and ungetch (.) to be
 * aliases for the underscored names.
 *
 * For uniformity, I treat getche () and kbhit () the same way.
 *
 * (Even with this, it is probably still a bad idea to use both <conio.h>
 * and <curses.h> in the same C module.)
 */
extern int _getch (void);
extern int _getche (void);
extern int _kbhit (void);
extern int _ungetch (int);

extern int _LIBI86_REDIRECT (getch, (void), _getch);
extern int _LIBI86_REDIRECT (getche, (void), _getche);
extern int _LIBI86_REDIRECT (kbhit, (void), _kbhit);
extern int _LIBI86_REDIRECT (ungetch, (int), _ungetch);

_LIBI86_END_EXTERN_C

#endif
