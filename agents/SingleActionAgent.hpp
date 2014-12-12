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
 *  SingleActionAgent.hpp
 *
 * The implementation of the SingleActionAgent class. With probability epsilon
 *  it takes a random action; otherwise it takes the action specified by the 
 *  configuration under 'agent_action'.
 **************************************************************************** */

#ifndef __SINGLE_ACTION_AGENT_HPP__
#define __SINGLE_ACTION_AGENT_HPP__

#include "common/Constants.h"
#include "PlayerAgent.hpp"
#include "emucore/OSystem.hxx"

namespace ale {

class SingleActionAgent : public PlayerAgent {
    public:
        SingleActionAgent(OSystem * _osystem, RomSettings * _settings);
		
	protected:
        /* *********************************************************************
            Returns the best action from the set of possible actions
         ******************************************************************** */
        virtual Action act();

  protected:
      double epsilon;
      Action agent_action;
};

} // namespace ale

#endif // __SINGLE_ACTION_AGENT_HPP__
 

