/*
 * Copyright (c) 2019--2021 TK Chia
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

#ifndef _LIBI86_DPMI_H_
#define _LIBI86_DPMI_H_

#include <libi86/internal/cdefs.h>
#include <stdbool.h>
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

typedef struct __attribute__ ((__packed__))
  {
    uint8_t accessed : 1, rdwr : 1, exp_down: 1, execute : 1,
	    mustbe_1 : 1, dpl : 2, present : 1;
  }
dsc_type;

typedef struct __attribute__ ((__packed__))
  {
    uint8_t : 4, useravail : 1, mustbe_0 : 1, use32 : 1, page_gran : 1;
  }
dsc_xtype;

typedef struct __attribute__ ((__packed__))
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

typedef struct __attribute__ ((__packed__))
  {
    union
      {
	uint32_t edi;
	uint16_t di;
      };
    union
      {
	uint32_t esi;
	uint16_t si;
      };
    union
      {
	uint32_t ebp;
	uint16_t bp;
      };
    uint32_t reserved;
    union
      {
	uint32_t ebx;
	uint16_t bx;
      };
    union
      {
	uint32_t edx;
	uint16_t dx;
      };
    union
      {
	uint32_t ecx;
	uint16_t cx;
      };
    union
      {
	uint32_t eax;
	uint16_t ax;
      };
    uint16_t flags, es, ds, fs, gs, ip, cs, sp, ss;
  }
rm_call_struct;

typedef struct
  {
    uint16_t rm, pm;
  }
dpmi_dos_block;

typedef struct
  {
    uint8_t ver_major, ver_minor;
    char vendor[126];
  }
dpmi_host_info;

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

_LIBI86_ALT_INLINE int32_t
_DPMIAllocateLDTDescriptors (uint16_t __count)
{
  uint16_t __sel;
  int __res;
  __asm volatile ("int {$}0x31; sbb{w} %1, %1"
		  : "=a" (__sel), "=cr" (__res)
		  : "0" (0x0000U), "c" (__count)
		  : "cc", "memory");
  return (int32_t) __res << 16 | __sel;
}

_LIBI86_ALT_INLINE int
_DPMIFreeDOSMemoryBlock (uint16_t __sel)
{
  int __res;
  __asm volatile ("int {$}0x31; sbb{w} %0, %0"
		  : "=adr" (__res)
		  : "a" (0x0101U), "d" (__sel)
		  : "cc", "memory");
  return __res;
}

_LIBI86_ALT_INLINE int
_DPMIFreeLDTDescriptor (uint16_t __sel)
{
  int __res;
  __asm volatile ("int {$}0x31; sbb{w} %0, %0"
		  : "=abr" (__res)
		  : "a" (0x0001U), "b" (__sel)
		  : "cc", "memory");
  return __res;
}

_LIBI86_ALT_INLINE int
#ifdef __FAR
_DPMIGetDescriptor (uint16_t __sel, descriptor __far *__desc)
#else
_DPMIGetDescriptor (uint16_t __sel, __libi86_fpv_t __desc)
#endif
{
  int __res;
  __asm volatile ("int {$}0x31; sbb{w} %0, %0"
		  : "=abr" (__res)
		  : "a" (0x000bU), "b" (__sel),
		    "e" (FP_SEG (__desc)), "D" (FP_OFF (__desc))
		  : "cc", "memory");
  return __res;
}

_LIBI86_ALT_INLINE int
#ifdef __FAR
_DPMIGetCapabilities (uint16_t *__caps1, uint16_t *__caps2,
		      uint16_t *__caps3, dpmi_host_info __far *__host)
#else
_DPMIGetCapabilities (uint16_t *__caps1, uint16_t *__caps2,
		      uint16_t *__caps3, __libi86_fpv_t __host)
#endif
{
  int __res;
  uint16_t __c1, __c2, __c3;
  __asm volatile ("int {$}0x31; sbb{w} %0, %0"
		  : "=br" (__res), "=a" (__c1), "=c" (__c2), "=d" (__c3)
		  : "a" (0x0401U), "e" (FP_SEG (__host)), "D" (FP_OFF (__host))
		  : "cc", "memory");
  if (! __res)
    {
      *__caps1 = __c1;
      *__caps2 = __c2;
      *__caps3 = __c3;
    }
  return __res;
}

_LIBI86_ALT_INLINE uint32_t
_DPMIGetSegmentBaseAddress (uint16_t __sel)
{
  uint16_t __xx, __hi, __lo;
  __asm volatile ("int {$}0x31"
		  : "=a" (__xx), "=c" (__hi), "=d" (__lo)
		  : "0" (0x0006U), "b" (__sel), "1" (~0U), "2" (~0U)
		  : "cc");
  return (uint32_t) __hi << 16 | __lo;
}

_LIBI86_ALT_INLINE __libi86_fpcv_t
_DPMIGetVendorSpecificAPI (__libi86_fpcc_t __vendor)
{
  __libi86_segment_t __segm;
  unsigned __offs;
  uint8_t __res;
  /*
   * "DPMI 1.0 clients should use this function [int 0x2f, ax = 0x168a] in
   *  preference to Int 31H Function 0A00H. ...  Note that although this
   *  function was not documented for DPMI 0.9, it will work under any DPMI
   *  0.9 host."	-- DPMI Specification 1.0
   */
  __asm volatile ("int {$}0x2f"
		  : "=Ral" (__res), "=e" (__segm), "=D" (__offs)
		  : "a" (0x168aU),
		    "Rds" (FP_SEG (__vendor)), "S" (FP_OFF (__vendor)),
		    "1" (__builtin_ia16_selector (0U)), "2" (0U)
		  : "cc");
  if (__res)
    return MK_FP (0, 0);
  return MK_FP (__segm, __offs);
}

_LIBI86_ALT_INLINE bool
_DPMIGetVirtualInterruptState (void)
{
  uint8_t __res;
  __asm volatile ("int {$}0x31" : "=Ral" (__res) : "a" (0x0902U) : "cc");
  return __res != 0;
}

_LIBI86_ALT_INLINE uint8_t
_DPMIIdle (void)
{
  uint16_t __res;
  __asm volatile ("int {$}0x2f" : "=a" (__res) : "0" (0x1680U)
				: "cc", "memory");
  return (uint8_t) __res;
}

_LIBI86_ALT_INLINE int32_t
_DPMISegmentToDescriptor (uint16_t __para)
{
  uint16_t __sel;
  int __res;
 __asm volatile ("int {$}0x31; sbb{w} %1, %1"
		 : "=a" (__sel), "=br" (__res)
		 : "0" (0x0002U), "b" (__para)
		 : "cc", "memory");
  if (__res < 0)
    return -1L;
  return (int32_t) __sel;
}

_LIBI86_ALT_INLINE int
#ifdef __FAR
_DPMISetDescriptor (uint16_t __sel, const descriptor __far *__desc)
#else
_DPMISetDescriptor (uint16_t __sel, __libi86_fpcv_t __desc)
#endif
{
  int __res;
  __asm volatile ("int {$}0x31; sbb{w} %0, %0"
		  : "=abr" (__res)
		  : "a" (0x000cU), "b" (__sel),
		    "e" (FP_SEG (__desc)), "D" (FP_OFF (__desc))
		  : "cc", "memory");
  return __res;
}

_LIBI86_ALT_INLINE int
_DPMISimulateRealModeInterrupt (uint8_t __intr_no, uint8_t __flags,
				uint16_t __words_to_copy,
				rm_call_struct __far *__call_st)
{
  int __res;
  __asm volatile ("int {$}0x31; sbb{w} %0, %0"
		  : "=abcr" (__res)
		  : "a" (0x0300U),
		    "b" ((uint16_t) __flags << 8 | __intr_no),
		    "c" (__words_to_copy),
		    "e" (FP_SEG (__call_st)), "D" (FP_OFF (__call_st))
		  : "cc", "memory");
  return __res;
}

_LIBI86_END_EXTERN_C

#endif
