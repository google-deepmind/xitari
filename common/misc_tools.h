/* *****************************************************************************
 * Xitari
 *
 * Copyright 2014 Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 *  misc_tools.h
 *
 *  A set of miscellaneous tools used in various places.
 **************************************************************************** */
#ifndef __MISC_TOOLS_H__
#define __MISC_TOOLS_H__

#include "Constants.h"

#ifndef WIN32
#include <sys/time.h>
#endif

namespace ale {

/* *****************************************************************************
    Inline C++ integer exponentiation routines 
    Version 1.01
    Copyright (C) 1999-2004 John C. Bowman <bowman@math.ualberta.ca>
 **************************************************************************** */
inline int pow(int x, int p) {
    if(p == 0) return 1;
    if(x == 0 && p > 0) return 0;
    if(p < 0) {assert(x == 1 || x == -1); return (-p % 2) ? x : 1;}

    int r = 1;
    for(;;) {
    if(p & 1) r *= x;
    if((p >>= 1) == 0)  return r;
    x *= x;
    }
}

/* *****************************************************************************
    Makes x fit within the [uper, lower] bounds
 **************************************************************************** */
inline void bound(int& x, int lower_bound, int upper_bound) {
    if (x > upper_bound) {
        x = upper_bound;
    }
    if (x < lower_bound) {
        x = lower_bound;
    }
}

/* *****************************************************************************
    Return time in milliseconds. 
 **************************************************************************** */
#ifndef WIN32

inline long timeMillis() {

    struct timeval ts; 
  gettimeofday(&ts, NULL);
  return ts.tv_sec * 1000 + ts.tv_usec/1000;
}

#else

#include <windows.h>
inline long timeMillis() {
    return GetTickCount();
}

#endif

} // namespace ale

#endif // __MISC_TOOLS_H__
