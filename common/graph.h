/*
 * Copyright (c) 2020 TK Chia
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

#ifndef _LIBI86_GRAPH_H_
#define _LIBI86_GRAPH_H_

#define _DEFAULTMODE	(-1)
#define _TEXTBW40	0x0000
#define _TEXTC40	0x0001
#define _TEXTBW80	0x0002
#define _TEXTC80	0x0003
#define _MRES4COLOR	0x0004
#define _MRESNOCOLOR	0x0005
#define _HRESBW		0x0006
#define _TEXTMONO	0x0007
#define _HERCMONO	0x000b
#define _MRES16COLOR	0x000d
#define _HRES16COLOR	0x000e
#define _ERESNOCOLOR	0x000f
#define _ERESCOLOR	0x0010
#define _VRES2COLOR	0x0011
#define _VRES16COLOR	0x0012
#define _MRES256COLOR	0x0013
#define _URES256COLOR	0x0100
#define _VRES256COLOR	0x0101
#define _SVRES16COLOR	0x0102
#define _SVRES256COLOR	0x0103
#define _XRES16COLOR	0x0104
#define _XRES256COLOR	0x0105
#define _YRES16COLOR	0x0106
#define _YRES256COLOR	0x0107
#define _SVTEXTC80X60	0x0108
#define _SVTEXTC132X25	0x0109
#define _SVTEXTC132X43	0x010a
#define _SVTEXTC132X50	0x010b
#define _SVTEXTC132X60	0x010c
#define _MRES32KCOLOR	0x010d
#define _MRES64KCOLOR	0x010e
#define _MRESTRUECOLOR	0x010f
#define _VRES32KCOLOR	0x0110
#define _VRES64KCOLOR	0x0111
#define _VRESTRUECOLOR	0x0112
#define _SVRES32KCOLOR	0x0113
#define _SVRES64KCOLOR	0x0114
#define _SVRESTRUECOLOR	0x0115
#define _XRES32KCOLOR	0x0116
#define _XRES64KCOLOR	0x0117
#define _XRESTRUECOLOR	0x0118
#define _YRES32KCOLOR	0x0119
#define _YRES64KCOLOR	0x011a
#define _YRESTRUECOLOR	0x011b
#define _ZRES256COLOR	0x011c
#define _ZRES32KCOLOR	0x011d
#define _ZRES64KCOLOR	0x011e
#define _ZRESTRUECOLOR	0x011f

#define _GCLEARSCREEN	0
#define _GVIEWPORT	1
#define _GWINDOW	2

/*
 * Some Open Watcom routines accept colour codes in the form of RGB triplets,
 * as well as indexed colour codes --- unfortunately with no good way to
 * distinguish them in some cases.
 *
 * Here I define constants for RGB triplets with a bit set in a high
 * position, so that they can be more easily differentiated from indexed
 * colour codes.
 */
#define _BLACK		0x800000L
#define _BLUE		(_BLACK | 0x2a0000L)
#define _GREEN		(_BLACK | 0x002a00L)
#define _CYAN		(_BLACK | 0x2a2a00L)
#define _RED		(_BLACK | 0x00002aL)
#define _MAGENTA	(_BLACK | 0x2a002aL)
#define _BROWN		(_BLACK | 0x00152aL)
#define _WHITE		(_BLACK | 0x2a2a2aL)
#define _GRAY		(_BLACK | 0x151515L)
#define _LIGHTBLUE	(_BLACK | 0x3f1515L)
#define _LIGHTGREEN	(_BLACK | 0x153f15L)
#define _LIGHTCYAN	(_BLACK | 0x3f3f15L)
#define _LIGHTRED	(_BLACK | 0x15153fL)
#define _LIGHTMAGENTA	(_BLACK | 0x3f153fL)
#define _YELLOW		(_BLACK | 0x153f3fL)
#define _LIGHTYELLOW	_YELLOW
#define _BRIGHTWHITE	(_BLACK | 0x3f3f3fL)

#ifndef __ASSEMBLER__

# include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

typedef short grcolor;

enum
{
  _GCURSOROFF, _GCURSORON
};

#define _GSCROLLUP	((short) 1)
#define _GSCROLLDOWN	((short) -1)

struct rccoord
{
  short row, col;
};

extern void _clearscreen (short __area);
extern short _displaycursor (short __curs_mode);
extern struct rccoord _gettextposition (void);
extern short _getvideomode (void);
extern void _outmem (__libi86_fpcc_t __text, short __length);
extern void _outtext (__libi86_fpcc_t __text);
extern void _scrolltextwindow (short __rows);
extern grcolor _setcolor (grcolor __pixval);
extern struct rccoord _settextposition (short __row, short __col);
extern short _setvideomode (short __mode);
extern grcolor _settextcolor (grcolor __pixval);
extern void _settextwindow (short __row1, short __col1,
			    short __row2, short __col2);

extern short __libi86_displaycursor_off (short __curs_mode);
extern short __libi86_displaycursor_on (short __curs_mode);
extern long __libi86_setbkcolor_rgb (long __color);
extern long __libi86_setbkcolor_indexed (long __pixval);

# if ! defined __GNUC__ || ! defined __OPTIMIZE__
extern short _displaycursor (short __curs_mode);
extern long _setbkcolor (long __color);
# else  /* __GNUC__ && __OPTIMIZE__ */
extern short __libi86_displaycursor (short __curs_mode);
extern long __libi86_setbkcolor (long __color);

#   ifndef _LIBI86_COMPILING_
_LIBI86_ALT_INLINE short
_displaycursor (short __curs_mode)
{
  if (! __builtin_constant_p (__curs_mode))
    return __libi86_displaycursor (__curs_mode);
  else if (__curs_mode == _GCURSOROFF)
    return __libi86_displaycursor_off (__curs_mode);
  else
    return __libi86_displaycursor_on (__curs_mode);
}

_LIBI86_ALT_INLINE _Bool
__libi86_rgb_color_p (long __color)
{
  return __color < 0 /* ?!? */ || __color > (long) 0xff;
}

_LIBI86_ALT_INLINE long
_setbkcolor (long __color)
{
  if (! __builtin_constant_p (__color))
    return __libi86_setbkcolor (__color);
  else if (__libi86_rgb_color_p (__color))
    return __libi86_setbkcolor_rgb (__color);
  else
    return __libi86_setbkcolor_indexed (__color);
}
#   endif  /* ! _LIBI86_COMPILING_ */
# endif  /* __GNUC__ && __OPTIMIZE__ */

_LIBI86_END_EXTERN_C
#endif  /* ! __ASSEMBLER__ */

#endif
