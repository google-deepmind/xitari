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
 *  internal_controller.hpp
 *
 *  The InternalController class allows users to directly interface with ALE.
 *
 **************************************************************************** */

#ifndef __INTERNAL_CONTROLLER_HPP__
#define __INTERNAL_CONTROLLER_HPP__

#include "ale_controller.hpp"
#include "agents/PlayerAgent.hpp"

namespace ale {

class InternalController : public ALEController {
  public:
    InternalController(OSystem* osystem);
    virtual ~InternalController() {}

    virtual void run();

  private:
    bool isDone();
    void createAgents();

    void episodeEnd();
    void episodeStart(Action& action_a, Action& action_b);
    void episodeStep(Action& action_a, Action& action_b);

  private:
    int m_max_num_frames; // Maximum number of total frames before we stop
    int m_max_num_episodes; // Maximum number of episodes before we stop

    int m_episode_score; // Keeping track of score
    int m_episode_number; // Keeping track of episode 

    std::auto_ptr<PlayerAgent> m_agent_left; // Agents 
    std::auto_ptr<PlayerAgent> m_agent_right; 
};

} // namespace ale

#endif // __INTERNAL_CONTROLLER_HPP__
