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
 *  common_constants.h
 *
 *  Defines a set of constants used by various parts of the player agent code
 *
 **************************************************************************** */

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <cassert>
#include <vector>
#include <valarray>
#include <cstdlib>
#include "emucore/m6502/src/bspf/src/bspf.hxx"
#include "ale_interface.hpp"

namespace ale {

#define PLAYER_A_MAX (18)
#define PLAYER_B_MAX (36)

std::string action_to_string(Action a);

//  Define datatypes
typedef std::vector<int>         IntVect;
typedef std::vector<double>      FloatVect;
typedef std::vector<IntVect>     IntMatrix;
typedef std::vector<FloatVect>   FloatMatrix;

// return (sum of discounted rewards) type for RL
typedef float return_t;

// Other constant values
#define RAM_LENGTH 128
#define CUSTOM_PALETTE_SIZE 1020    // Number of colors in custom palette
#define BLACK_COLOR_IND 1000        // color index in custom palette for Black
#define RED_COLOR_IND 1001          // color index in custom palette for Red
#define WHITE_COLOR_IND 1003        // color index in custom palette for White
#define SECAM_COLOR_IND 1010        // starting index in the custom palette for
                                    // the eight SECAM colors

} // namespace ale

#endif // __CONSTANTS_H__

