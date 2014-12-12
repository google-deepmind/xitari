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
 *  SingleActionAgent.cpp
 *
 * The implementation of the SingleActionAgent class. With probability epsilon
 *  it takes a random action; otherwise it takes the action specified by the 
 *  configuration under 'agent_action'.
 **************************************************************************** */

#include "SingleActionAgent.hpp"
#include "common/random_tools.h"

using namespace ale;

SingleActionAgent::SingleActionAgent(OSystem* _osystem, RomSettings* _settings) : PlayerAgent(_osystem, _settings) {
    epsilon = _osystem->settings().getFloat("agent_epsilon", true);
    agent_action = (Action)_osystem->settings().getInt("agent_action", true);
}

Action SingleActionAgent::act() {

  double r = double(rand()) / double(RAND_MAX); 

  if (r < epsilon)
    return choice(&available_actions);
  else
    return agent_action; 
}

