/*
 * Copyright (c) 2020 TK Chia
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

#ifndef __ASSEMBLER__

# include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

#define _GSCROLLUP	((short) 1)
#define _GSCROLLDOWN	((short) -1)

struct rccoord
{
  short row, col;
};

extern struct rccoord _gettextposition (void);
extern short _getvideomode (void);
extern void _scrolltextwindow (short __rows);

/* Used by the inline version of _setvideomode (.) below.  */
extern short __libi86_setvideomode (short);
extern short __libi86_setvideomode_default (void);
extern short __libi86_setvideomode_mdpa_cga (short);
extern short __libi86_setvideomode_nonsvga (short);
extern short __libi86_setvideomode_svga (short);

# ifndef __OPTIMIZE__
extern short _setvideomode (short);
# else  /* __OPTIMIZE__ */
__attribute__ ((always_inline)) static short
__libi86_setvideomode_inline_dispatch (short mode)
{
  switch (mode)
    {
    case _DEFAULTMODE:
      return __libi86_setvideomode_default ();

    case (short) 0x0000 ... (short) 0x007f:
      return __libi86_setvideomode_nonsvga (mode);

    case (short) 0x0080 ... (short) 0x01ff:
      return __libi86_setvideomode_svga (mode);

    default:
      return 0;
    }
}

#   ifndef _LIBI86_COMPILING_
_LIBI86_ALT_INLINE short
_setvideomode (short mode)
{
  if (__builtin_constant_p (mode))
    return __libi86_setvideomode_inline_dispatch (mode);
  else
    return __libi86_setvideomode (mode);
}
#   endif
# endif  /* __OPTIMIZE__ */

_LIBI86_END_EXTERN_C
#endif  /* ! __ASSEMBLER__ */

#endif
