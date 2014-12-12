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
 *  stella_environment.cpp
 *
 *  A class that wraps around the Stella core to provide users with a typical
 *  reinforcement learning environment interface.
 *  
 **************************************************************************** */

#include "stella_environment.hpp"
#include "../emucore/m6502/src/System.hxx"
#include <cstring>

using namespace ale;

StellaEnvironment::StellaEnvironment(OSystem* osystem, RomSettings* settings):
  m_osystem(osystem),
  m_settings(settings),
  m_phosphor_blend(osystem),
  m_screen(m_osystem->console().mediaSource().height(),
        m_osystem->console().mediaSource().width()) {

  // Determine whether this is a paddle-based game
  if (m_osystem->console().properties().get(Controller_Left) == "PADDLES" ||
      m_osystem->console().properties().get(Controller_Right) == "PADDLES") {
    m_use_paddles = true;
    m_state.resetPaddles(m_osystem->event());
  } else {
    m_use_paddles = false;
  }

  m_cartridge_md5 = m_osystem->console().properties().get(Cartridge_MD5);
  
  m_num_reset_steps = atoi(m_osystem->settings().getString("system_reset_steps").c_str());
  m_use_starting_actions = m_osystem->settings().getBool("use_starting_actions");
  
  m_max_num_frames_per_episode = m_osystem->settings().getInt("max_num_frames_per_episode");
  m_colour_averaging = !m_osystem->settings().getBool("disable_color_averaging");

  m_backward_compatible_save = m_osystem->settings().getBool("backward_compatible_save");
  m_stochastic_start = m_osystem->settings().getBool("use_environment_distribution");
}

/** Resets the system to its start state. */
void StellaEnvironment::reset() {
  // RNG for generating environments
  Random randGen;

  // Reset the paddles
  m_state.resetVariables(m_osystem->event());

  // Reset the emulator
  m_osystem->console().system().reset();

  // NOOP for 60 steps in the deterministic environment setting, or some random amount otherwise 
  int noopSteps;
  if (m_stochastic_start)
    noopSteps = 60 + rand() % NUM_RANDOM_ENVIRONMENTS;
  else
    noopSteps = 60;

  emulate(PLAYER_A_NOOP, PLAYER_B_NOOP, noopSteps);
  // reset for n steps
  emulate(RESET, PLAYER_B_NOOP, m_num_reset_steps);

  // reset the rom (after emulating, in case the NOOPs led to reward)
  m_settings->reset();
  
  // Apply necessary actions specified by the rom itself
  if (m_use_starting_actions) {
    ActionVect startingActions = m_settings->getStartingActions();
    for (size_t i = 0; i < startingActions.size(); i++)
      emulate(startingActions[i], PLAYER_B_NOOP);
  }
}

/** Save/restore the environment state. */
void StellaEnvironment::save() {
  // Store the current state into a new object
  ALEState new_state = m_state.save(m_osystem, m_settings, m_cartridge_md5);

  if (m_backward_compatible_save) { // 0.2, 0.3: overwrite on save
    while (!m_saved_states.empty())
      m_saved_states.pop();
    m_saved_states.push(new_state);
  }
  else { // 0.4 and above: put it on the stack
    m_saved_states.push(new_state);
  }
}

/** Get a copy of the underlying environment state. */
ALEState *StellaEnvironment::cloneState() const {

    // cast away const, to deal with legacy saving code not properly 
    // handling it. should be fine.
    ALEState *state = const_cast<ALEState *>(&m_state);

    // note: there is an unnecessary copy, due to state->save returning a full object, 
    //       which we can avoid later if performance is an issue.
    ALEState *rval = new ALEState(state->save(m_osystem, m_settings, m_cartridge_md5));

    return rval;
}

/** Restore the environment to a previously saved state. */
void StellaEnvironment::restoreState(const ALEState &state) {

    // Deserialize it into 'm_state'
    m_state.load(m_osystem, m_settings, m_cartridge_md5, state);
}

/** Destroy a cloned state. */
void StellaEnvironment::destroyState(const ALEState *state) const {
    if (state != NULL) delete state;
}

bool StellaEnvironment::load() {

  if (m_saved_states.empty()) return false;  

  // Get the state on top of the stack
  ALEState& target_state = m_saved_states.top(); 
 
  // Deserialize it into 'm_state'
  m_state.load(m_osystem, m_settings, m_cartridge_md5, target_state);

  if (m_backward_compatible_save) { // 0.2, 0.3: persistent save 
  }
  else { // 0.4 and above: take it off the stack
    m_saved_states.pop();
  }

  return true;
}

void StellaEnvironment::noopIllegalActions(Action & player_a_action, Action & player_b_action) {
  if (player_a_action < (Action)PLAYER_B_NOOP && 
        !m_settings->isLegal(player_a_action)) {
    player_a_action = (Action)PLAYER_A_NOOP;
  }
  // Also drop RESET, which doesn't play nice with our clean notions of RL environments
  else if (player_a_action == RESET) 
    player_a_action = (Action)PLAYER_A_NOOP;

  if (player_b_action < (Action)RESET && 
        !m_settings->isLegal((Action)((int)player_b_action - PLAYER_B_NOOP))) {
    player_b_action = (Action)PLAYER_B_NOOP;
  }
  else if (player_b_action == RESET) 
    player_b_action = (Action)PLAYER_B_NOOP;
}

/** Applies the given actions (e.g. updating paddle positions when the paddle is used)
  *  and performs one simulation step in Stella. */
reward_t StellaEnvironment::act(Action player_a_action, Action player_b_action) {
  // Once in a terminal state, refuse to go any further (special actions must be handled
  //  outside of this environment; in particular reset() should be called rather than passing
  //  RESET or SYSTEM_RESET.
  if (isTerminal())
    return 0;

  // Convert illegal actions into NOOPs; actions such as reset are always legal
  noopIllegalActions(player_a_action, player_b_action);
  
  // Emulate in the emulator
  emulate(player_a_action, player_b_action);
  m_state.incrementFrame(); 

  return m_settings->getReward();
}

bool StellaEnvironment::isTerminal() const {
  return (m_settings->isTerminal() || 
    (m_max_num_frames_per_episode > 0 && 
     m_state.getEpisodeFrameNumber() >= m_max_num_frames_per_episode) ||
    (m_settings->maxFrames() > 0 &&
     m_state.getEpisodeFrameNumber() >= m_settings->maxFrames()));
}

void StellaEnvironment::emulate(Action player_a_action, Action player_b_action, size_t num_steps) {
  Event* event = m_osystem->event();
  
  // Handle paddles separately: we have to manually update the paddle positions at each step
  if (m_use_paddles) {
    // Run emulator forward for 'num_steps'
    for (size_t t = 0; t < num_steps; t++) {
      // Update paddle position at every step
      m_state.applyActionPaddles(event, player_a_action, player_b_action);

      m_osystem->console().mediaSource().update();
      m_settings->step(m_osystem->console().system());
    }
  }
  else {
    // In joystick mode we only need to set the action events once
    m_state.setActionJoysticks(event, player_a_action, player_b_action);

    for (size_t t = 0; t < num_steps; t++) {
      m_osystem->console().mediaSource().update();
      m_settings->step(m_osystem->console().system());
    }
  }

  // Parse screen and RAM into their respective data structures
  processScreen();
  processRAM();
}

/** Accessor methods for the environment state. */
void StellaEnvironment::setState(const ALEState& state) {
  m_state = state;
}

const ALEState& StellaEnvironment::getState() const {
  return m_state;
}

void StellaEnvironment::processScreen() {
  if (!m_colour_averaging) {
    // Copy screen over and we're done! 
    int size = m_osystem->console().mediaSource().width() * m_osystem->console().mediaSource().height();
    assert(size == m_screen.getArray().size());
    uInt8* frame_buffer = m_osystem->console().mediaSource().currentFrameBuffer();
    std::copy(frame_buffer,frame_buffer + size , m_screen.getArray().begin());
  }
  else {
    // Perform phosphor averaging; the blender stores its result in the given screen
    m_phosphor_blend.process(m_screen);
  }
}

void StellaEnvironment::processRAM() {
  // Copy RAM over
  for (size_t i = 0; i < m_ram.size(); i++) {
    unsigned int idx = static_cast<unsigned int>(i);
    uInt16 idx2 = static_cast<uInt16>(i + 0x80);
    *m_ram.byte(idx) = m_osystem->console().system().peek(idx2);
  }
}

