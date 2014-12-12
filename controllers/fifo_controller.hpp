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
 *  fifo_controller.hpp
 *
 *  The FIFOController class implements an Agent/ALE interface via stdin/stdout
 *  or named pipes.
 **************************************************************************** */

#ifndef __FIFO_CONTROLLER_HPP__
#define __FIFO_CONTROLLER_HPP__

#include "ale_controller.hpp"

namespace ale {

class FIFOController : public ALEController {
  public:
    FIFOController(OSystem* osystem, bool named_pipes = false);
    virtual ~FIFOController();

    virtual void run();

  private:
    void handshake(); // Perform handshaking
    void openNamedPipes();

    bool isDone();
    void sendData();
    void readAction(Action& action_a, Action& action_b);

    void sendScreen();
    int stringScreenRLE(const ALEScreen& screen, char * buffer);
    int stringScreenFull(const ALEScreen& screen, char * buffer);
    void sendRAM();
    void sendRL();

  private:
    bool m_named_pipes; // Whether to use named pipes

    int m_max_num_frames; // Maximum number of total frames before we stop
    bool m_run_length_encoding; // Whether to encode the data in a run-length fashion

    int m_frame_skip;  // Requested frame skip
    bool m_send_screen; // Agent requested screen data
    bool m_send_ram; // Agent requested RAM data
    bool m_send_rl; // Agent requested RL data
    
    FILE* m_fout; 
    FILE* m_fin; 

};

} // namespace ale

#endif // __FIFO_CONTROLLER_HPP__
