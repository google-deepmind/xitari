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
 * RomSettings.cpp
 *
 * The interface to describe games as RL environments. It provides terminal and
 *  reward information.
 * *****************************************************************************
 */
#include "RomSettings.hpp"

using namespace ale;

bool RomSettings::isLegal(const Action& a) const {
  return true;
}

ActionVect& RomSettings::getMinimalActionSet() {
  if (actions.empty()) {
    for (int a = 0; a < PLAYER_B_NOOP; a++)
      if (isMinimal((Action)a) && isLegal((Action)a))
        actions.push_back((Action)a);
  }

  return actions;
}

ActionVect& RomSettings::getAllActions() {
  // Generate the set of all actions
  if (all_actions.empty()) {
    for (int a = 0; a < PLAYER_B_NOOP; a++)
      if (isLegal((Action)a))
        all_actions.push_back((Action)a);
  }

  return all_actions;
}

ActionVect RomSettings::getStartingActions() {
    return ActionVect();
}
