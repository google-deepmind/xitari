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
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare,
 *  Matthew Hausknecht, and the Reinforcement Learning and Artificial Intelligence 
 *  Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 *  ale_interface.hpp
 *
 *  The static/shared library interface.
 **************************************************************************** */

#ifndef __ALE_INTERFACE_HPP__
#define __ALE_INTERFACE_HPP__

#include <iostream>
#include <vector>
#include <memory>
#include <cassert>

namespace ale {


class OSystem;
class Event;
class Settings;
struct RomSettings;
class StellaEnvironment;


// Define possible actions
enum Action {
    PLAYER_A_NOOP           = 0,
    PLAYER_A_FIRE           = 1,
    PLAYER_A_UP             = 2,
    PLAYER_A_RIGHT          = 3,
    PLAYER_A_LEFT           = 4,
    PLAYER_A_DOWN           = 5,
    PLAYER_A_UPRIGHT        = 6,
    PLAYER_A_UPLEFT         = 7,
    PLAYER_A_DOWNRIGHT      = 8,
    PLAYER_A_DOWNLEFT       = 9,
    PLAYER_A_UPFIRE         = 10,
    PLAYER_A_RIGHTFIRE      = 11,
    PLAYER_A_LEFTFIRE       = 12,
    PLAYER_A_DOWNFIRE       = 13,
    PLAYER_A_UPRIGHTFIRE    = 14,
    PLAYER_A_UPLEFTFIRE     = 15,
    PLAYER_A_DOWNRIGHTFIRE  = 16,
    PLAYER_A_DOWNLEFTFIRE   = 17,
    PLAYER_B_NOOP           = 18,
    PLAYER_B_FIRE           = 19,
    PLAYER_B_UP             = 20,
    PLAYER_B_RIGHT          = 21,
    PLAYER_B_LEFT           = 22,
    PLAYER_B_DOWN           = 23,
    PLAYER_B_UPRIGHT        = 24,
    PLAYER_B_UPLEFT         = 25,
    PLAYER_B_DOWNRIGHT      = 26,
    PLAYER_B_DOWNLEFT       = 27,
    PLAYER_B_UPFIRE         = 28,
    PLAYER_B_RIGHTFIRE      = 29,
    PLAYER_B_LEFTFIRE       = 30,
    PLAYER_B_DOWNFIRE       = 31,
    PLAYER_B_UPRIGHTFIRE    = 32,
    PLAYER_B_UPLEFTFIRE     = 33,
    PLAYER_B_DOWNRIGHTFIRE  = 34,
    PLAYER_B_DOWNLEFTFIRE   = 35,
    RESET                   = 40, // note: we use SYSTEM_RESET instead to reset the environment. 
    UNDEFINED               = 41,
    RANDOM                  = 42,
    SAVE_STATE              = 43,
    LOAD_STATE              = 44,
    SYSTEM_RESET            = 45,
    SELECT                  = 46, // Used to select game mode... should only be used internally 
    LAST_ACTION_INDEX       = 50
};

// a list of ALE actions
typedef std::vector<Action> ActionVect;

// type used to represent insantanteous reward
typedef int reward_t;

typedef unsigned char byte_t;
typedef unsigned char pixel_t;


/** A simple wrapper around an Atari screen. */ 
class ALEScreen { 
    public:
        ALEScreen(int height, int width) : m_height(height), m_width(width), m_pixels(static_cast<size_t>(m_height * m_width)) {}
        ALEScreen(const ALEScreen &rhs) : m_height(rhs.m_height), m_width(rhs.m_width), m_pixels(rhs.m_pixels) {}
        const ALEScreen& operator=(const ALEScreen &rhs);

        /** pixel accessors, (row, column)-ordered */
        const pixel_t& pixel(int r, int c) const { return m_pixels[index(r, c)]; }
        pixel_t& pixel(int r, int c) { return m_pixels[index(r, c)]; }
    
        /** Access the whole array */
        const std::vector<pixel_t>& getArray() const { return m_pixels; }
        std::vector<pixel_t>& getArray() { return m_pixels; }

        /** Dimensionality information */
        int height() const { return m_height; }
        int width() const { return m_width; }

        /** Returns the size of the underlying array */
        size_t arraySize() const { return m_pixels.size() * sizeof(pixel_t); }

        /** Returns whether two screens are equal */
        // note that we use short circuit evaluation
        bool equals(const ALEScreen &rhs) const 
            { return width() == rhs.width() && height() == rhs.height() 
                && m_pixels == rhs.getArray(); }

  private:
        size_t index(int r,int c) const 
          { assert(r < height()); assert(c < width()); return static_cast<size_t>(r * width() + c); }

        int m_height;
        int m_width;
        std::vector<pixel_t> m_pixels; 

};


// A simple wrapper around the 1024 bit Atari RAM.
class ALERAM { 
    
    public:

        ALERAM();
        ALERAM(const ALERAM &rhs);
        ALERAM &operator=(const ALERAM &rhs);

        // Byte accessors.
        byte_t get(unsigned int x) const;
        byte_t *byte(unsigned int x);
   
        /** Returns a pointer to the internal Atari RAM. */
        byte_t *array() const { return (byte_t *) m_ram; }

        /** Return the size of the internal Atari memory in bytes. */
        size_t size() const { return sizeof(m_ram); }

        /** Returns whether two copies of the RAM are equal. */
        bool equals(const ALERAM &rhs) const;

  private:

        byte_t m_ram[128];
};


// This class provides a simplified interface to ALE.
class ALEInterface {

    public:

        /** create an ALEInterface. This routine is not threadsafe! 
            One also has the option of creating a single Atari session
            that will randomly (uniform) alternate between a number of
            different ROM files. The syntax is:  
                <rom path>+<rom path>+... */
        ALEInterface(const std::string &rom_file);
        
        /** Unload the emulator. */
        ~ALEInterface();

        /** Resets the game. */
        void resetGame();

        /** Indicates if the game has ended. */
        bool gameOver() const;

        /** Applies an action to the game and returns the reward. It is the user's responsibility
            to check if the game has ended and reset when necessary - this method will keep pressing
            buttons on the game over screen. */
        reward_t act(Action action);

        /** Returns the vector of legal actions. */
        ActionVect getLegalActionSet();

        /** Returns a vector describing the minimal set of actions needed to play current game. */
        ActionVect getMinimalActionSet();

        /** Returns the frame number since the loading of the ROM. */
        int getFrameNumber() const;

        /** Set the frame limit for each episode. '0' means no limit. */
        void setMaxNumFrames(int newMax);

        /** Minimum possible instantaneous reward. */
        reward_t minReward() const;

        /** Maximum possible instantaneous reward. */
        reward_t maxReward() const;

        /** The remaining number of lives. */
        int lives() const;

        /** Returns the frame number since the start of the current episode. */
        int getEpisodeFrameNumber() const;

        /** Returns a handle to the current game screen. */
        const ALEScreen &getScreen() const;

        /** Writes the screen out to a PNG. */
        bool screenToPNG(const std::string &filename);

        /** Access the current emulator memory state. */
        const ALERAM &getRAM() const;

        /** Saves the state of the emulator system, overwriting any 
            previously saved state. */
        void saveState();

        /** Restores a previously saved state of the emulator system,
            returns false if no such state exists (and makes no changes
            to the emulator system). */
        bool loadState();

        /** Gets a state as a string. */
        std::string getSnapshot() const;

        /** Sets the state from a string*/
        void restoreSnapshot(const std::string& snapshot);

        /** OSystem accessor. */
        const OSystem &osystem() const;
        
        /** Get the current version of the ALE interface.
            Major versions indicate significant changes that might break backward compatibility.
            Minor versions indicate bug-fixes. */
        void getVersion(int &major, int &minor) const;

        /** Converts a pixel to its RGB value. */
        static void getRGB(
            unsigned char pixel, 
            unsigned char &red, 
            unsigned char &green, 
            unsigned char &blue
        );

    private:

        /** Copying is explicitly disallowed. */
        ALEInterface(const ALEInterface &);

        /** Assignment is explicitly disallowed. */
        ALEInterface &operator=(const ALEInterface &);

        class Impl;
        Impl *m_pimpl;
};


/** Creates an emulator system. Used only by standalone Ale process. */
extern void createOSystem(
    int argc, 
    char* argv[],
    OSystem* &theOSystem,
    Settings* &theSettings
);

} // namespace ale

#endif // __ALE_INTERFACE_HPP__
