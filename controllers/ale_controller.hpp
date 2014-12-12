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
 *  ale_controller.hpp 
 *
 *  Superclass defining a variety of controllers -- main loops interfacing with
 *   an agent in a particular way. This superclass handles work common to all 
 *   controllers, e.g. loading ROM settings and constructing the environment
 *   wrapper.
 **************************************************************************** */
#ifndef __ALE_CONTROLLER_HPP__
#define __ALE_CONTROLLER_HPP__

#include "emucore/OSystem.hxx"
#include "emucore/m6502/src/System.hxx"
#include "environment/stella_environment.hpp"

namespace ale {

class ALEController {
  public:
    ALEController(OSystem * osystem);
    virtual ~ALEController() {}

    /** Main loop. Returns once ALE terminates. */
    virtual void run() = 0;

  protected:
    friend class ALEInterface;

    /** Applies the given action to the environment (e.g. by emulating or resetting) */
    void applyActions(Action a, Action b); 
    /** Support for SDL display... available to all controllers. Simply call it from run(). */
    void display();

  protected:
    OSystem* m_osystem;
    std::auto_ptr<RomSettings> m_settings;
    StellaEnvironment m_environment;
};

} // namespace ale

#endif // __ALE_CONTROLLER_HPP__
