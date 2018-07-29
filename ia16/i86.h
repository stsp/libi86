/*
 * Copyright (c) 2018 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING3.  If not see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef _I86_H_
#define _I86_H_

struct WORDREGS
{
  unsigned short ax, _1, bx, _2, cx, _3, dx, _4, si, _5, di, _6, cflag;
};

struct BYTEREGS
{
};

#endif
