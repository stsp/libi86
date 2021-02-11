/*
 * Copyright (c) 2019--2020 TK Chia
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

#ifndef _LIBI86_DPMI_H_
#define _LIBI86_DPMI_H_

#include <libi86/internal/cdefs.h>
#include <stdint.h>
#include "i86.h"

_LIBI86_BEGIN_EXTERN_C

/*
 * __DPMI_hosted () is from Watcom's internally-used interface.
 *
 * If the C runtime (e.g. Newlib), or the program using libi86, defines
 * a __DPMI_hosted () function, then we use it.  We can check at `configure'
 * time whether the C runtime has this function.
 *
 * If neither the libc or the application has this function, then this
 * library provides a "default" implementation, which works but is not very
 * efficient.  This inefficient implementation is available via the name
 * __libi86_DPMI_hosted (), for testing purposes.
 */
extern int __DPMI_hosted (void);
extern int __libi86_DPMI_hosted (void);

/*
 * These follow Watcom's internally-used interface, & are defined in this
 * very library.
 */

typedef struct __attribute__ ((packed))
  {
    uint8_t accessed : 1, rdwr : 1, exp_down: 1, execute : 1,
	    mustbe_1 : 1, dpl : 2, present : 1;
  }
dsc_type;

typedef struct __attribute__ ((packed))
  {
    uint8_t : 4, useravail : 1, mustbe_0 : 1, use32 : 1, page_gran : 1;
  }
dsc_xtype;

typedef struct __attribute__ ((packed))
  {
    uint16_t lim_0_15, base_0_15;
    uint8_t base_16_23;
    dsc_type type;
    union
      {
	struct
	  {
	    uint8_t lim_16_19 : 4, : 4;
	  };
	dsc_xtype xtype;
      };
    uint8_t base_24_31;
  }
descriptor;

typedef struct __attribute__ ((packed))
  {
    uint32_t edi, esi, ebp, reserved, ebx, edx, ecx, eax;
    uint16_t flags, es, ds, fs, gs, ip, cs, sp, ss;
  }
rm_call_struct;

typedef struct
  {
    uint16_t rm, pm;
  }
dpmi_dos_block;

_LIBI86_ALT_INLINE dpmi_dos_block
_DPMIAllocateDOSMemoryBlock (uint16_t __paras)
{
  dpmi_dos_block __blk;
  int __xx;
  __asm volatile ("int {$}0x31; "
		  "jnc 0f; "
		  "xor{w} %0, %0; "
		  "cw{t}d; "
		  "0:"
		  : "=a" (__blk.rm), "=d" (__blk.pm), "=b" (__xx)
		  : "0" (0x0100U), "2" (__paras)
		  : "cc", "memory");
  return __blk;
}

_LIBI86_ALT_INLINE int
_DPMIFreeDOSMemoryBlock (uint16_t __sel)
{
  int __res;
  __asm volatile ("int {$}0x31; sbb{w} %0, %0"
		  : "=a" (__res)
		  : "0" (0x0101U), "d" (__sel)
		  : "cc", "memory");
  return __res;
}

_LIBI86_ALT_INLINE int
#ifdef __FAR
_DPMIGetDescriptor (uint16_t __sel, descriptor __far *__desc)
#else
_DPMIGetDescriptor (uint16_t __sel, __libi86_fpv __desc)
#endif
{
  int __res;
  __asm volatile ("int {$}0x31; sbb{w} %0, %0"
		  : "=a" (__res)
		  : "0" (0x000bU), "b" (__sel),
		    "e" (FP_SEG (__desc)), "D" (FP_OFF (__desc))
		  : "cc", "memory");
  return __res;
}

_LIBI86_ALT_INLINE int32_t
_DPMISegmentToDescriptor (uint16_t __para)
{
  uint16_t __sel;
  int __res;
 __asm volatile ("int {$}0x31; sbb{w} %1, %1"
		 : "=a" (__sel), "=r" (__res)
		 : "0" (0x0002U), "b" (__para)
		 : "cc", "memory");
  if (__res < 0)
    return -1L;
  return (int32_t) __sel;
}

_LIBI86_ALT_INLINE int
_DPMISimulateRealModeInterrupt (uint8_t __interrupt, uint8_t __flags,
				uint16_t __words_to_copy,
				rm_call_struct __far *__call_st)
{
  int __res;
  __asm volatile ("int {$}0x31; sbb{w} %0, %0"
		  : "=a" (__res)
		  : "0" (0x0300U),
		    "b" ((uint16_t) __flags << 8 | __interrupt),
		    "c" (__words_to_copy),
		    "e" (FP_SEG (__call_st)), "D" (FP_OFF (__call_st))
		  : "cc", "memory");
  return __res;
}

_LIBI86_END_EXTERN_C

#endif
