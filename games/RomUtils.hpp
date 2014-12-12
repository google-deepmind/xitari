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
 *
 * RomUtils.hpp
 *
 * Additional utilities to operate on RAM.
 * *****************************************************************************
 */
#ifndef __ROMUTILS_HPP__
#define __ROMUTILS_HPP__

namespace ale {

class System;

// reads a byte at a memory location between 0 and 1023
extern int readRam(const System* system, int offset);

// extracts a decimal value from 1, 2, and 3 bytes respectively
extern int getDecimalScore(int idx, const System* system);
extern int getDecimalScore(int lo, int hi, const System* system);
extern int getDecimalScore(int lo, int mid, int hi, const System* system);

} // namespace ale

#endif // __ROMUTILS_HPP__



