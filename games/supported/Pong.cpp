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
 */
#include "Pong.hpp"

#include "../RomUtils.hpp"

using namespace ale;


PongSettings::PongSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* PongSettings::clone() const { 
    
    RomSettings* rval = new PongSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void PongSettings::step(const System& system) {

    // update the reward
    int x = readRam(&system, 13); // cpu score
    int y = readRam(&system, 14); // player score
    reward_t score = y - x;
    m_reward = score - m_score;
    m_score = score;

    // update terminal status
    // game is over when a player reaches 21,
    // or too many steps have elapsed since the last reset
    m_terminal = x == 21 || y == 21;
}

// we need to lower the episode length further for Pong
int PongSettings::maxFrames() const { 
    return 65000; 
}

/* is end of game */
bool PongSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t PongSettings::getReward() const { 

    return m_reward; 
}


/* is an action part of the minimal set? */
bool PongSettings::isMinimal(const Action &a) const {

    switch (a) {
        case PLAYER_A_NOOP:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
            return true;
        default:
            return false;
    }   
}


/* reset the state of the game */
void PongSettings::reset() {
    
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
}

        
/* saves the state of the rom settings */
void PongSettings::saveState(Serializer & ser) {
    
    ser.putInt(m_reward);
    ser.putInt(m_score);
    ser.putBool(m_terminal);
}


/* loads the state of the rom settings */
void PongSettings::loadState(Deserializer & ser) {
  
    m_reward = ser.getInt();
    m_score = ser.getInt();
    m_terminal = ser.getBool();
}

