/** @file main.cpp */

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
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <memory>

#include "emucore/m6502/src/bspf/src/bspf.hxx"
#include "emucore/Console.hxx"
#include "emucore/Event.hxx"
#include "emucore/PropsSet.hxx"
#include "emucore/Settings.hxx"
#include "emucore/FSNode.hxx"
#include "emucore/OSystem.hxx"

#include "common/Defaults.hpp"

#ifdef WIN32
#   include "os_dependent/SettingsWin32.hxx"
#   include "os_dependent/OSystemWin32.hxx"
#else
#   include "os_dependent/SettingsUNIX.hxx"
#   include "os_dependent/OSystemUNIX.hxx"
#endif

#include "controllers/ale_controller.hpp"
#include "controllers/fifo_controller.hpp"
#include "controllers/rlglue_controller.hpp"
#include "controllers/internal_controller.hpp"
#include "common/Constants.h"
#include "ale_interface.hpp"

using namespace ale;


static OSystem *theOSystem = NULL;
static Settings *theSettings = NULL;


static const std::string Version = "0.4";


// Display welcome message
static std::string welcomeMessage() {
    // ALE welcome message
    std::ostringstream oss;
    oss << "A.L.E: Arcade Learning Environment (version "
        << Version << ")\n"
        << "[Powered by Stella]\n"
        << "Use -help for help screen.";
    return oss.str();
}


static void disableBufferedIO() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    std::cin.rdbuf()->pubsetbuf(0,0);
    std::cout.rdbuf()->pubsetbuf(0,0);
    std::cin.sync_with_stdio();
    std::cout.sync_with_stdio();
}


static ALEController* createController(OSystem* osystem, std::string type) {
  if (type == "fifo") {
    std::cerr << "Game will be controlled through FIFO pipes." << std::endl;
    return new FIFOController(osystem, false);
  }
  else if (type == "fifo_named") {
    std::cerr << "Game will be controlled through named FIFO pipes." << std::endl;
    return new FIFOController(osystem, true);
  }
  else if (type == "rlglue") {
    std::cerr << "Game will be controlled through RL-Glue." << std::endl;
    return new RLGlueController(osystem);
  }
  else if (type == "internal") {
    std::cerr << "Game will be controlled by an internal agent." << std::endl;
    return new InternalController(osystem);
  }
  else {
    std::cerr << "Invalid controller type: " << type << " " << std::endl;
    exit(1);
  }
}


/* application entry point */
int main(int argc, char* argv[]) {

    try {

        disableBufferedIO();

        std::cerr << welcomeMessage() << std::endl;

        createOSystem(argc, argv, theOSystem, theSettings);

        // Create the game controller
        std::string controller_type = theOSystem->settings().getString("game_controller");
        std::auto_ptr<ALEController> controller(createController(theOSystem, controller_type));

        controller->run();

        // MUST delete theOSystem to avoid a segfault (theOSystem relies on Settings
        //  still being a valid construct)
        delete theOSystem;
        delete theSettings;

    } catch (std::exception &e) {

        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

