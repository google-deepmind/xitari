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
 *  controller.hpp 
 *
 *  Superclass defining a variety of controllers -- main loops interfacing with
 *   an agent in a particular way. This superclass handles work common to all 
 *   controllers, e.g. loading ROM settings and constructing the environment
 *   wrapper.
 **************************************************************************** */

#include "ale_controller.hpp"
#include "games/Roms.hpp"

#include "common/display_screen.h"

using namespace ale;

ALEController::ALEController(OSystem* osystem):
  m_osystem(osystem),
  m_settings(buildRomRLWrapper(m_osystem->settings().getString("rom_file"))),
  m_environment(m_osystem, m_settings.get()) {

  if (m_settings.get() == NULL) {
    std::cerr << "Unsupported ROM file: " << std::endl;
    exit(1);
  }
  else {
    applyRomSettings(m_settings.get(), m_osystem);
    m_environment.reset();
  }
}

void ALEController::display() {
  DisplayScreen* display = m_osystem->p_display_screen;

  if (display) // Display the screen if applicable
    display->display_screen(m_osystem->console().mediaSource());
}

void ALEController::applyActions(Action player_a, Action player_b) {
  // Perform different operations based on the first player's action 
  switch (player_a) {
    case LOAD_STATE: // Load system state
      // Note - this does not reset the game screen; so that the subsequent screen
      //  is incorrect (in fact, two screens, due to colour averaging)
      m_environment.load();
      break;
    case SAVE_STATE: // Save system state
      m_environment.save();
      break;
    case SYSTEM_RESET:
      m_environment.reset();
      break;
    default:
      // Pass action to emulator!
      m_environment.act(player_a, player_b);
      break;
  }
}

