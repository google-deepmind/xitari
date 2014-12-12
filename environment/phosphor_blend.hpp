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
 *  phosphor_blend.hpp
 *
 *  Methods for performing colour averaging over the screen.
 *  
 **************************************************************************** */

#ifndef __PHOSPHOR_BLEND_HPP__
#define __PHOSPHOR_BLEND_HPP__

#include "emucore/OSystem.hxx"
#include "ale_interface.hpp"

namespace ale {

class PhosphorBlend {
  public:
    PhosphorBlend(OSystem *);

    void process(ALEScreen& screen);

  private:
    void makeAveragePalette();
    uInt8 getPhosphor(uInt8 v1, uInt8 v2);
    uInt32 makeRGB(uInt8 r, uInt8 g, uInt8 b);
    /** Converts a RGB value to an 8-bit format */
    uInt8 rgbToNTSC(uInt32 rgb);
    
  private:
    OSystem * m_osystem;

    uInt8 m_rgb_ntsc[64][64][64];

    uInt32 m_avg_palette[256][256];
    uInt8 m_phosphor_blend_ratio;
};

} // namespace ale

#endif // __PHOSPHOR_BLEND_HPP__

