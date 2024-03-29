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

#ifndef _LIBI86_LIBI86_INTERNAL_GRAPH_H_
#define _LIBI86_LIBI86_INTERNAL_GRAPH_H_

#ifndef _LIBI86_COMPILING_
# error "<libi86/internal/graph.h> should only be used when compiling libi86!"
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <libi86/internal/bda.h>
#include <libi86/internal/cdefs.h>
#include <libi86/internal/farptr.h>
#include <graph.h>
#include <i86.h>

_LIBI86_BEGIN_EXTERN_C

/*
 * Various internal functions, variables, & macros used by <graph.h> routines.
 */

/* State variables used for both text & graphics output. */
struct __libi86_vid_state_t
{
  /* Current video mode number. */
  unsigned mode_num;
  /* Video mode at program startup. */
  unsigned startup_mode_num;
  /*
   * Maximum possible extent of text window, i.e. the actual display
   * dimensions, minus 1.  The <graph.h> interface uses 16-bit shortwords,
   * but internally we just use bytes (for now).
   */
  unsigned char max_x, max_y;
  /*
   * Current text window --- top left corner & bottom right corner.  These
   * are 0-based, i.e. the top left corner of the physical screen is (0, 0).
   */
  unsigned char x1z, y1z, x2z, y2z;
  /* Current colour attribute for text output. */
  unsigned char attribute;
  /* Active video adapter type. */
  char adapter;
  /* Whether we are in a graphics mode. */
  unsigned graph_p : 1;
  /*
   * Whether the cursor is supposed to be displayed.  This will be a hardware
   * cursor in a text mode, & a software cursor in a graphics mode.
   */
  unsigned curs_p : 1;
  /* Background colour. */
  long bk_colr;
};

/* State variables used only for graphics output. */
struct __libi86_graph_state_t
{
  /* Current drawing colour. */
  unsigned char draw_colr;
  /* Maximum colour value. */
  unsigned char max_colr;
};

/*
 * Internal (row, column) co-ordinates, in character units.  This must
 * _exactly_ match the data format used in the BIOS data area.  The top left
 * corner of the physical screen is (0, 0).
 */
struct __libi86_vid_rccoord_t
{
  unsigned char x, y;
};

union __libi86_vid_rccoord_union_t
{
  struct __libi86_vid_rccoord_t pxy;
  uint16_t w;
};

extern struct __libi86_vid_state_t __libi86_vid_state;
extern struct __libi86_graph_state_t __libi86_graph_state;

/* Get the BIOS's idea of the current display page. */
_LIBI86_STATIC_INLINE unsigned char
__libi86_vid_get_curr_pg (void)
{
  return __libi86_peekb_bda (0x0062U);
}

/*
 * Get the BIOS's idea of the current cursor position for the display page
 * PG_NO.
 */
_LIBI86_STATIC_INLINE struct __libi86_vid_rccoord_t
__libi86_vid_get_rccoord (unsigned char pg_no)
{
  union __libi86_vid_rccoord_union_t u;
  u.w = __libi86_peek_bda (0x0050U + 2 * pg_no);
  return u.pxy;
}

extern struct __libi86_vid_rccoord_t __libi86_vid_get_and_adjust_rccoord
				       (unsigned char pg_no);
#ifndef __GNUC__
extern void __libi86_vid_state_init (void);
#endif

/*
 * Change the BIOS's idea of the current cursor position for the display
 * page PG_NO, without actually moving the displayed cursor.
 */
_LIBI86_STATIC_INLINE void
__libi86_vid_set_rccoord_only (unsigned char pg_no,
			       struct __libi86_vid_rccoord_t pxy)
{
  union __libi86_vid_rccoord_union_t u;
  u.pxy = pxy;
  __libi86_poke_bda (0x0050U + 2 * pg_no, u.w);
}

#ifndef __GNUC__
extern uint32_t __libi86_vid_int_0x10 (uint16_t, uint16_t, uint16_t, uint16_t);
#endif

/* Really set the cursor position for the display page PG_NO. */
_LIBI86_STATIC_INLINE void
__libi86_vid_go_rccoord (unsigned char pg_no,
			 struct __libi86_vid_rccoord_t pxy)
{
#ifdef __GNUC__
  unsigned xx1, xx2, xx3;
  __asm volatile ("int {$}0x10" : "=a" (xx1), "=b" (xx2), "=d" (xx3)
				: "Rah" ((uint8_t) 0x02),
				  "1" ((uint16_t) pg_no << 8),
				  "2" (pxy)
				: "cc", "cx", "memory");
#else
  union __libi86_vid_rccoord_union_t u;
  u.pxy = pxy;
  __libi86_vid_int_0x10 (0x0200U, (uint16_t) pg_no << 8, 0, u.w);
#endif
}

/* Get the current hardware cursor shape. */
#ifndef __GNUC__
extern unsigned __libi86_vid_get_curs_shape (void);
extern uint32_t __libi86_vid_get_ega_info (void);
extern uint8_t __libi86_vid_get_ega_pal_reg (uint8_t, uint8_t);
extern void __libi86_vid_set_ega_pal_reg (uint8_t, uint8_t);
extern long __libi86_vid_get_vga_dac_reg (uint8_t);
extern void __libi86_vid_set_vga_dac_reg (uint8_t, long);
#else
/* Get the hardware cursor shape. */
_LIBI86_STATIC_INLINE unsigned
__libi86_vid_get_curs_shape (void)
{
  unsigned ax, bx, shape;
  __asm volatile ("int {$}0x10" : "=a" (ax), "=b" (bx), "=c" (shape)
				: "Rah" ((uint8_t) 0x03), "b" (0U)
				: "cc", "dx", "memory");
  return shape;
}

/* Get EGA information. */
_LIBI86_STATIC_INLINE uint32_t
__libi86_vid_get_ega_info (void)
{
  unsigned ax, bx, cx;
  __asm volatile ("int {$}0x10" : "=a" (ax), "=b" (bx), "=c" (cx)
				: "Rah" ((uint8_t) 0x12), "1" (0xff10U)
				: "cc", "dx", "memory");
  return (uint32_t) bx << 16 | cx;
}

/*
 * Get the value of an individual EGA palette register.  If the BIOS does
 * not allow this, return a given default value.
 */
_LIBI86_STATIC_INLINE uint8_t
__libi86_vid_get_ega_pal_reg (uint8_t idx, uint8_t deflt)
{
  unsigned ax, bx;
  __asm volatile ("int {$}0x10" : "=a" (ax), "=b" (bx)
				: "0" (0x1007U),
				  "1" ((uint16_t) deflt << 8 | idx)
				: "cc", "cx", "dx", "memory");
  return (uint8_t) (bx >> 8);
}

/* Set an individual EGA palette register. */
_LIBI86_STATIC_INLINE void
__libi86_vid_set_ega_pal_reg (uint8_t idx, uint8_t val)
{
  unsigned ax, bx;
  __asm volatile ("int {$}0x10" : "=a" (ax), "=b" (bx)
				: "0" (0x1000U),
				  "1" ((uint16_t) val << 8 | idx)
				: "cc", "cx", "dx", "memory");
}

/* Get the value of an individual VGA or MCGA palette register. */
_LIBI86_STATIC_INLINE long
__libi86_vid_get_vga_dac_reg (uint8_t idx)
{
  unsigned ax, bx, cx, dx;
  uint8_t r, g, b;
  __asm volatile ("int {$}0x10" : "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx)
				: "0" (0x1015U), "b" ((uint16_t) idx));
  r = (uint8_t) (dx >> 8);
  g = (uint8_t) (cx >> 8);
  b = (uint8_t) cx;
  return _BLACK | r | (long) g << 8 | (long) b << 16;
}

/* Set an individual VGA or MCGA palette register. */
_LIBI86_STATIC_INLINE void
__libi86_vid_set_vga_dac_reg (uint8_t idx, long pixval)
{
  uint8_t r = (uint8_t) pixval & (uint8_t) 0x3f,
	  g = (uint8_t) (pixval >> 8) & (uint8_t) 0x3f,
	  b = (uint8_t) (pixval >> 16) & (uint8_t) 0x3f;
  unsigned ax, bx, cx, dx;
  __asm volatile ("int {$}0x10" : "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx)
				: "0" (0x1010U), "b" ((uint16_t) idx),
				  "c" ((uint16_t) g << 8 | b), "Rdh" (r));
}
#endif

/* Set the hardware cursor shape. */
_LIBI86_STATIC_INLINE void
__libi86_vid_set_curs_shape (unsigned shape)
{
#ifdef __GNUC__
  unsigned ax, cx;
  __asm volatile ("int {$}0x10" : "=a" (ax), "=c" (cx)
				: "Rah" ((uint8_t) 0x01), "1" (shape)
				: "cc", "bx", "dx", "memory");
#else
  __libi86_vid_int_0x10 (0x0100U, 0, shape, 0);
#endif
}

/* Beep. */
_LIBI86_STATIC_INLINE void
__libi86_vid_beep (unsigned char pg_no, unsigned char attr)
{
#ifdef __GNUC__
  unsigned xx1, xx2;
  __asm volatile ("pushw %%bp; int $0x10; popw %%bp"
		  : "=a" (xx1), "=b" (xx2)
		  : "0" (0x0e00U | '\a'), "1" ((uint16_t) pg_no << 8 | attr)
		  : "cc", "cx", "dx", "memory");
#else
  __libi86_vid_int_0x10 (0x0e00U | '\a', (uint16_t) pg_no << 8 | attr, 0, 0);
#endif
}

/* Plot a character & attribute at the current cursor position in a page. */
_LIBI86_STATIC_INLINE void
__libi86_vid_plot_char (unsigned char pg_no, unsigned char ch,
			unsigned char attr)
{
#ifdef __GNUC__
  unsigned xx1, xx2, xx3;
  __asm volatile ("int $0x10" : "=a" (xx1), "=b" (xx2), "=c" (xx3)
			      : "Rah" ((unsigned char) 0x09), "Ral" (ch),
				"1" ((uint16_t) pg_no << 8 | attr),
				"2" (1U)
			      : "cc", "dx", "memory");
#else
  __libi86_vid_int_0x10 (0x0900U | ch, (uint16_t) pg_no << 8 | attr, 1U, 0);
#endif
}

/*
 * Update the given cursor position to scroll to the next line in the text
 * window.  Scroll the window area up if necessary.
 */
_LIBI86_STATIC_INLINE struct __libi86_vid_rccoord_t
__libi86_vid_next_line (struct __libi86_vid_rccoord_t pxy)
{
  if (pxy.y < __libi86_vid_state.y2z)
    ++pxy.y;
  else
    _scrolltextwindow (_GSCROLLUP);
  pxy.x = __libi86_vid_state.x1z;
  return pxy;
}

extern void __libi86_vid_xor_sw_cursor (void);

_LIBI86_STATIC_INLINE void
__libi86_vid_stop_sw_cursor (void)
{
  __libi86_vid_xor_sw_cursor ();
}

_LIBI86_STATIC_INLINE void
__libi86_vid_resume_sw_cursor (void)
{
  __libi86_vid_xor_sw_cursor ();
}

/*
 * Common code for implementing _outmem (, ), _outtext (.), & Borland-like
 * putch (.), cputs (.), etc.
 *
 * Under GCC, this is an inline function which is transformed into
 * specialized code for _outmem (, ) etc.
 *
 * Under ACK, this will be compiled as a normal out-of-line function, in a
 * .c source file that defines the macro _LIBI86_COMPILING_VID_OUTMEM_DO_.
 */
#if ! defined __GNUC__ && ! defined _LIBI86_COMPILING_VID_OUTMEM_DO_
extern void __libi86_vid_outmem_do (__libi86_fpcc_t, size_t, bool, bool);
#else  /* __GNUC__ || _LIBI86_COMPILING_VID_OUTMEM_DO_ */
# ifdef __GNUC__
static inline
# endif
void
__libi86_vid_outmem_do (__libi86_fpcc_t text, size_t length,
			bool handle_cr_lf_p, bool handle_bel_bs_p)
{
  unsigned char pg_no;
  unsigned char x1z, x2z, attr;
  struct __libi86_vid_rccoord_t pxy;

  if (! length)
    return;

# ifndef __GNUC__
  __libi86_vid_state_init ();
# endif

  __libi86_vid_stop_sw_cursor ();

  /* Get our current text window & output text colour attribute. */
  x1z = __libi86_vid_state.x1z;
  x2z = __libi86_vid_state.x2z;
  attr = __libi86_vid_state.attribute;

  /*
   * Get the current cursor position according to the BIOS.  If the cursor
   * is outside the window, mentally move it inside.
   */
  pg_no = __libi86_vid_get_curr_pg ();
  pxy = __libi86_vid_get_and_adjust_rccoord (pg_no);

  /* Display the characters.  Scroll the window as necessary. */
  while (length-- != 0)
    {
# ifdef __FAR
      char ch = *text++;
# else
      char ch = __libi86_peekfpbi (&text);
# endif

      if (handle_cr_lf_p)
	{
	  switch (ch)
	    {
	    case '\r':
	      pxy.x = x1z;
	      continue;
	    case '\n':
	      pxy = __libi86_vid_next_line (pxy);
	      continue;
	    default:
	      ;
	    }
	}

      if (handle_bel_bs_p)
	{
	  switch (ch)
	    {
	    case '\a':
	      /* Remember to move the physical cursor before beeping... */
	      __libi86_vid_go_rccoord (pg_no, pxy);
	      __libi86_vid_resume_sw_cursor ();
	      /* Beep. */
	      __libi86_vid_beep (pg_no, attr);
	      /*
	       * If the '\a' is the last character, we can just return.
	       * Otherwise carry on.
	       */
	      if (! length)
		return;
	      __libi86_vid_stop_sw_cursor ();
	      continue;
	    case '\b':
	      if (pxy.x > x1z)
		--pxy.x;
	      continue;
	    default:
	      ;
	    }
	}

      __libi86_vid_set_rccoord_only (pg_no, pxy);
      __libi86_vid_plot_char (pg_no, ch, attr);

      if (pxy.x >= x2z)
	pxy = __libi86_vid_next_line (pxy);
      else
	++pxy.x;
    }

  /* Move the cursor to where it should now be. */
  __libi86_vid_go_rccoord (pg_no, pxy);
  __libi86_vid_resume_sw_cursor ();
}
#endif  /* __GNUC__ || _LIBI86_COMPILING_VID_OUTMEM_DO_ */

/* Scroll or clear an area of text characters on the current screen page. */
_LIBI86_STATIC_INLINE void
__libi86_vid_scroll (unsigned char sx1z, unsigned char sy1z,
		     unsigned char sx2z, unsigned char sy2z,
		     unsigned char rows, bool scroll_up_p)
{
  unsigned char func = scroll_up_p ? 0x06 : 0x07;
  unsigned char attr
    = __libi86_vid_state.graph_p ? 0 : __libi86_vid_state.attribute;
#ifdef __GNUC__
  unsigned xx1, xx2, xx3, xx4;
  __asm volatile ("pushw %%bp; int $0x10; popw %%bp"
		  : "=a" (xx1), "=b" (xx2), "=c" (xx3), "=d" (xx4)
		  : "Rah" (func), "Ral" (rows),
		    "1" ((unsigned) attr << 8),
		    "2" ((unsigned) sy1z << 8 | sx1z),
		    "Rdh" (sy2z), "Rdl" (sx2z));
#else
  __libi86_vid_int_0x10 ((unsigned) func << 8 | rows, (unsigned) attr << 8,
			 (unsigned) sy1z << 8 | sx1z,
			 (unsigned) sy2z << 8 | sx2z);
#endif
}

/*
 * For the time being, all supported video modes have character cells that are
 * 8 pixels wide...  -- tkchia 20211228
 */
_LIBI86_STATIC_INLINE unsigned char
__libi86_vid_get_cell_width (void)
{
  return (unsigned char) 8;
}

_LIBI86_STATIC_INLINE unsigned char
__libi86_vid_get_cell_height (void)
{
  unsigned char cell_ht = __libi86_peekb_bda (0x0085U);
  if (! cell_ht)
    cell_ht = 8;
  return cell_ht;
}

_LIBI86_STATIC_INLINE void
__libi86_vid_plot_pixel (unsigned gx, unsigned gy, unsigned char colour,
			 unsigned char pg_no)
{
#ifdef __GNUC__
  unsigned xx1, xx2, xx3, xx4;
  __asm volatile ("int {$}0x10"
		  : "=a" (xx1), "=b" (xx2), "=c" (xx3), "=d" (xx4)
		  : "Rah" ((uint8_t) 0xc), "Ral" (colour),
		    "1" ((uint16_t) pg_no << 8), "2" (gx), "3" (gy));
#else
  __libi86_vid_int_0x10 (0x0c00U | colour, (uint16_t) pg_no << 8, gx, gy);
#endif
}

_LIBI86_STATIC_INLINE uint8_t
__libi86_vid_get_pixel (unsigned gx, unsigned gy, unsigned char pg_no)
{
#ifdef __GNUC__
  unsigned ax, xx2, xx3, xx4;
  __asm volatile ("int {$}0x10"
		  : "=a" (ax), "=b" (xx2), "=c" (xx3), "=d" (xx4)
		  : "Rah" ((uint8_t) 0xd),
		    "1" ((uint16_t) pg_no << 8), "2" (gx), "3" (gy));
  return (uint8_t) ax;
#else
  return (uint8_t) __libi86_vid_int_0x10 (0x0d00U, (uint16_t) pg_no << 8,
					  gx, gy);
#endif
}

extern void __libi86_vid_bc_insdelline (bool);
extern void __libi86_vid_bc_outmem_do (const char *, size_t);
extern unsigned __libi86_con_mode_changed (unsigned);
extern void __libi86_vid_get_norm_attr (void);
extern void __libi86_graph_mode_changed (unsigned);
_LIBI86_END_EXTERN_C

#define _LIBI86_CASE_SUPPORTED_NONSVGA_TEXT_MODES \
	case _TEXTBW40:		\
	case _TEXTC40:		\
	case _TEXTBW80:		\
	case _TEXTC80:		\
	case _TEXTMONO:
#define _LIBI86_CASE_SUPPORTED_NONSVGA_2COLOR_GRAPHICS_MODES \
	case _HRESBW:		\
	case _HERCMONO:		\
	case _ERESNOCOLOR:	\
	case _VRES2COLOR:
#define _LIBI86_CASE_SUPPORTED_CGA_4COLOR_GRAPHICS_MODES \
	case _MRES4COLOR:	\
	case _MRESNOCOLOR:  /* 4 grays */
#define _LIBI86_CASE_SUPPORTED_NONSVGA_4COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_CGA_4COLOR_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_EGA_16COLOR_GRAPHICS_MODES \
	case _MRES16COLOR:	\
	case _HRES16COLOR:	\
	case _VRES16COLOR:
#define _LIBI86_CASE_SUPPORTED_NONSVGA_16COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_EGA_16COLOR_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_NONSVGA_SUB256COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_2COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_4COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_16COLOR_GRAPHICS_MODES \
	case _ERESCOLOR:  /* special case: may have 4 or 16 colours */
#define _LIBI86_CASE_SUPPORTED_NONSVGA_256COLOR_GRAPHICS_MODES \
	case _MRES256COLOR:
#define _LIBI86_CASE_SUPPORTED_NONSVGA_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_SUB256COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_256COLOR_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_SVGA_TEXT_MODES \
	case _SVTEXTC80X60:	\
	case _SVTEXTC132X25:	\
	case _SVTEXTC132X43:	\
	case _SVTEXTC132X50:	\
	case _SVTEXTC132X60:
#define _LIBI86_CASE_SUPPORTED_SVGA_16COLOR_GRAPHICS_MODES \
	case _SVRES16COLOR:	\
	case _XRES16COLOR:	\
	case _YRES16COLOR:
#define _LIBI86_CASE_SUPPORTED_SVGA_SUB256COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_SVGA_16COLOR_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_SVGA_256COLOR_GRAPHICS_MODES \
	case _URES256COLOR:	\
	case _VRES256COLOR:	\
	case _SVRES256COLOR:	\
	case _XRES256COLOR:	\
	case _YRES256COLOR:	\
	case _ZRES256COLOR:
#define _LIBI86_CASE_SUPPORTED_SVGA_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_SVGA_SUB256COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_SVGA_256COLOR_GRAPHICS_MODES \
	case _MRES32KCOLOR:	\
	case _MRES64KCOLOR:	\
	case _MRESTRUECOLOR:	\
	case _VRES32KCOLOR:	\
	case _VRES64KCOLOR:	\
	case _VRESTRUECOLOR:	\
	case _SVRES32KCOLOR:	\
	case _SVRES64KCOLOR:	\
	case _SVRESTRUECOLOR:	\
	case _XRES32KCOLOR:	\
	case _XRES64KCOLOR:	\
	case _XRESTRUECOLOR:	\
	case _YRES32KCOLOR:	\
	case _YRES64KCOLOR:	\
	case _YRESTRUECOLOR:	\
	case _ZRES32KCOLOR:	\
	case _ZRES64KCOLOR:	\
	case _ZRESTRUECOLOR:
#define _LIBI86_CASE_SUPPORTED_NONSVGA_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_TEXT_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_SVGA_MODES \
	_LIBI86_CASE_SUPPORTED_SVGA_TEXT_MODES \
	_LIBI86_CASE_SUPPORTED_SVGA_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_TEXT_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_TEXT_MODES \
	_LIBI86_CASE_SUPPORTED_SVGA_TEXT_MODES
#define _LIBI86_CASE_SUPPORTED_2COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_2COLOR_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_4COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_4COLOR_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_16COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_16COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_SVGA_16COLOR_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_SUB256COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_SUB256COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_SVGA_SUB256COLOR_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_256COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_256COLOR_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_SVGA_256COLOR_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_GRAPHICS_MODES \
	_LIBI86_CASE_SUPPORTED_SVGA_GRAPHICS_MODES
#define _LIBI86_CASE_SUPPORTED_MODES \
	_LIBI86_CASE_SUPPORTED_NONSVGA_MODES \
	_LIBI86_CASE_SUPPORTED_SVGA_MODES

#endif
