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
#include "Roms.hpp"
#include "RomSettings.hpp"
#include "RomUtils.hpp"
#include "emucore/OSystem.hxx"
#include <vector>

// include the game implementations
#include "supported/AirRaid.hpp"
#include "supported/Alien.hpp"
#include "supported/Amidar.hpp"
#include "supported/Assault.hpp"
#include "supported/Asterix.hpp"
#include "supported/Asteroids.hpp"
#include "supported/Atlantis.hpp"
#include "supported/BankHeist.hpp"
#include "supported/BattleZone.hpp"
#include "supported/BeamRider.hpp"
#include "supported/Berzerk.hpp"
#include "supported/Bowling.hpp"
#include "supported/Boxing.hpp"
#include "supported/Breakout.hpp"
#include "supported/Carnival.hpp"
#include "supported/Centipede.hpp"
#include "supported/ChopperCommand.hpp"
#include "supported/CrazyClimber.hpp"
#include "supported/Defender.hpp"
#include "supported/DemonAttack.hpp"
#include "supported/DoubleDunk.hpp"
#include "supported/ElevatorAction.hpp"
#include "supported/Enduro.hpp"
#include "supported/FishingDerby.hpp"
#include "supported/Freeway.hpp"
#include "supported/Frostbite.hpp"
#include "supported/Gopher.hpp"
#include "supported/Gravitar.hpp"
#include "supported/Hero.hpp"
#include "supported/IceHockey.hpp"
#include "supported/JamesBond.hpp"
#include "supported/JourneyEscape.hpp"
#include "supported/Kangaroo.hpp"
#include "supported/Krull.hpp"
#include "supported/KungFuMaster.hpp"
#include "supported/MontezumaRevenge.hpp"
#include "supported/MsPacman.hpp"
#include "supported/NameThisGame.hpp"
#include "supported/Pacman.hpp"
#include "supported/Phoenix.hpp"
#include "supported/Pitfall.hpp"
#include "supported/Pong.hpp"
#include "supported/Pooyan.hpp"
#include "supported/PrivateEye.hpp"
#include "supported/QBert.hpp"
#include "supported/RiverRaid.hpp"
#include "supported/RoadRunner.hpp"
#include "supported/RoboTank.hpp"
#include "supported/Seaquest.hpp"
#include "supported/Skiing.hpp"
#include "supported/Solaris.hpp"
#include "supported/SpaceInvaders.hpp"
#include "supported/StarGunner.hpp"
#include "supported/Surround.hpp"
#include "supported/Tennis.hpp"
#include "supported/TimePilot.hpp"
#include "supported/Tutankham.hpp"
#include "supported/UpNDown.hpp"
#include "supported/Venture.hpp"
#include "supported/VideoChess.hpp"
#include "supported/VideoPinball.hpp"
#include "supported/WizardOfWor.hpp"
#include "supported/YarsRevenge.hpp"
#include "supported/Zaxxon.hpp"


using namespace ale;


/* list of supported games */
static const RomSettings *roms[]  = {
    new AirRaidSettings(),
    new AlienSettings(),
    new AmidarSettings(),
    new AssaultSettings(),
    new AsterixSettings(),
    new AsteroidsSettings(),
    new AtlantisSettings(),
    new BankHeistSettings(),
    new BattleZoneSettings(),
    new BeamRiderSettings(),
    new BerzerkSettings(),
    new BowlingSettings(),
    new BoxingSettings(),
    new BreakoutSettings(),
    new CarnivalSettings(),
    new CentipedeSettings(),
    new ChopperCommandSettings(),
    new CrazyClimberSettings(),
    new DefenderSettings(),
    new DemonAttackSettings(),
    new DoubleDunkSettings(),
    new ElevatorActionSettings(),
    new EnduroSettings(),
    new FishingDerbySettings(),
    new FreewaySettings(),
    new FrostbiteSettings(),
    new GopherSettings(),
    new GravitarSettings(),
    new HeroSettings(),
    new IceHockeySettings(),
    new JamesBondSettings(),
    new JourneyEscapeSettings(),
    new KangarooSettings(),
    new KrullSettings(),
    new KungFuMasterSettings(),
    new MontezumaRevengeSettings(),
    new MsPacmanSettings(),
    new NameThisGameSettings(),
    new PacmanSettings(),
    new PhoenixSettings(),
    new PitfallSettings(),
    new PongSettings(),
    new PooyanSettings(),
    new PrivateEyeSettings(),
    new QBertSettings(),
    new RiverRaidSettings(),
    new RoadRunnerSettings(),
    new RoboTankSettings(),
    new SeaquestSettings(),
    new SkiingSettings(),
    new SolarisSettings(),
    new SpaceInvadersSettings(),
    new StarGunnerSettings(),
    new SurroundSettings(),
    new TennisSettings(),
    new TimePilotSettings(),
    new TutankhamSettings(),
    new UpNDownSettings(),
    new VentureSettings(),
    new VideoChessSettings(),
    new VideoPinballSettings(),
    new WizardOfWorSettings(),
    new YarsRevengeSettings(),
    new ZaxxonSettings(),
};

/* levenshtein function copied from
 * http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance
 */
using std::vector;
template <class T> unsigned int edit_distance(const T& s1, const T& s2)
{
    const size_t len1 = s1.size(), len2 = s2.size();
    vector<vector<unsigned int> > d(len1 + 1, vector<unsigned int>(len2 + 1));
 
    d[0][0] = 0;
    for(unsigned int i = 1; i <= len1; ++i) d[i][0] = i;
    for(unsigned int i = 1; i <= len2; ++i) d[0][i] = i;
 
    for(unsigned int i = 1; i <= len1; ++i)
        for(unsigned int j = 1; j <= len2; ++j)
 
                      d[i][j] = std::min( std::min(d[i - 1][j] + 1,d[i][j - 1] + 1),
                                          d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) );
    return d[len1][len2];
}


/* looks for the RL wrapper corresponding to a particular rom title */
RomSettings *ale::buildRomRLWrapper(const std::string &rom) {

    size_t slash_ind = rom.find_last_of("/\\");
    std::string rom_str = rom.substr(slash_ind + 1);
    size_t dot_idx = rom_str.find_first_of(".");
    rom_str = rom_str.substr(0, dot_idx);

    int closest_match_dist = 10000;
    std::string closest_match;
    int dist;

    for (size_t i=0; i < sizeof(roms)/sizeof(roms[0]); i++) {
        if (rom_str == roms[i]->rom()){
            printf("Found supported ROM: %s\n", roms[i]->rom());
            return roms[i]->clone();
        } else if((dist = edit_distance<std::string>(rom_str, roms[i]->rom())) < closest_match_dist){
            closest_match_dist = dist;
            closest_match = roms[i]->rom();
        }
    }

    fprintf(stderr, "no rom wrapper found for %s (searched for %s)\n", rom.c_str(), rom_str.c_str());
    if(closest_match_dist < rom.size())
        fprintf(stderr, "(is %s the ROM you meant to use?)\n", closest_match.c_str());
    return NULL;
}

void ale::applyRomSettings(RomSettings * settings, OSystem * osystem) {

    assert(settings != NULL && osystem != NULL);

    if (settings->swapPorts())
        osystem->console().toggleSwapPorts();
}
