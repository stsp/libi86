/*
 * Partial implementation of far pointers for GNU C++ for IA-16, for use by
 * libi86 modules and other libi86 header files.
 *
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

#ifndef _LIBI86_INTERNAL_FAR_CXX_H_
#define _LIBI86_INTERNAL_FAR_CXX_H_

namespace __libi86
{

typedef decltype (nullptr) __nullptr_t;

template <typename __T>
struct __is_void;

template <typename __T>
struct __is_void
{
  static const bool __value = false;
};

template <>
struct __is_void<void>
{
  static const bool __value = true;
};

template <>
struct __is_void<const void>
{
  static const bool __value = true;
};

template <typename __T>
class __far_ptr
{
  static_assert (__is_void<__T>::__value || __is_trivially_copyable (__T));
  static_assert (sizeof (__T *) == sizeof (unsigned));

  union
    {
      unsigned long __p_;
      struct
	{
	  __T *__off_;
	  unsigned __seg_;
	};
    };

public:
  __far_ptr ()
  : __off_ (0), __seg_ (0)
  {
  }

  __far_ptr (__nullptr_t)
  : __off_ (0), __seg_ (0)
  {
  }

  explicit
  __far_ptr (unsigned long __p)
  : __p_ (__p)
  {
  }

  __far_ptr (__unsigned seg, __T *__off)
  : __off_ (__off), __seg_ (__seg)
  {
  }

  __far_ptr (unsigned __seg, unsigned __off)
  : __off_ (reinterpret_cast<__T *> (__off)), __seg_ (__seg)
  {
  }

  __far_ptr (__T *__near_ptr)
  : __off_ (__near_ptr), __seg_ (__builtin_ia16_near_data_segment ())
  {
  }

  operator __far_ptr<void> ()
  {
    return __far_ptr<void> (__seg_, __off_);
  }

  template <typename __T2>
  explicit operator __far_ptr<__T2> ()
  {
    return __far_ptr<__T2> (__seg_, __off_);
  }

  unsigned
  __FP_SEG ()
  {
    return __seg_;
  }

  unsigned
  __FP_OFF ()
  {
    return reinterpret_cast<unsigned> (__off_);
  }
};

} /* namespace __libi86 */

typedef __libi86::__far_ptr<void> __libi86_fpv;
typedef __libi86::__far_ptr<const void> __libi86_fpcv;

#endif
