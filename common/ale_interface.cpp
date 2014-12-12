#include "ale_interface.hpp"

#include "emucore/FSNode.hxx"
#include "emucore/OSystem.hxx"
#include "os_dependent/SettingsWin32.hxx"
#include "os_dependent/OSystemWin32.hxx"
#include "os_dependent/SettingsUNIX.hxx"
#include "os_dependent/OSystemUNIX.hxx"
#include "games/Roms.hpp"
#include "common/Defaults.hpp"
#include "common/display_screen.h"
#include "environment/stella_environment.hpp"
#include "games/RomSettings.hpp"

#include <stdexcept>
#include <cstring>
#include <memory>
#include <cassert>
#include <vector>
#include <sstream>
#include <algorithm>


namespace ale {


// versioning information
static const int ALEMajorVersion = 1;
static const int ALEMinorVersion = 2;


void createOSystem(
    int argc,
    char* argv[],
    OSystem* &theOSystem,
    Settings* &theSettings
) {

    if (theOSystem != NULL) delete theOSystem;
    if (theSettings != NULL) delete theSettings;

#ifdef WIN32
    theOSystem = new OSystemWin32();
    theSettings = new SettingsWin32(theOSystem);
#else
    theOSystem = new OSystemUNIX();
    theSettings = new SettingsUNIX(theOSystem);
#endif

    setDefaultSettings(theOSystem->settings());

    theOSystem->settings().loadConfig();

    // process commandline arguments, which over-ride all possible config file settings
    std::string romfile = theOSystem->settings().loadCommandLine(argc, argv);
    if (romfile == "") std::exit(0);

    // Load the configuration from a config file (passed on the command
    //  line), if provided
    std::string configFile = theOSystem->settings().getString("config", false);

    if (!configFile.empty())
        theOSystem->settings().loadConfig(configFile.c_str());

    // disable about message
    theOSystem->settings().setBool("showinfo", false);

    theOSystem->settings().validate();
    theOSystem->create();

    std::string outputFile = theOSystem->settings().getString("output_file", false);
    if (!outputFile.empty()) {
        std::cerr << "Redirecting ... " << outputFile << std::endl;
        freopen(outputFile.c_str(), "w", stdout);
    }

    // attempt to load the ROM
    if (argc == 1 || romfile == "" || !FilesystemNode::fileExists(romfile)) {
      throw std::invalid_argument("could not find ROM file");
    } else if (theOSystem->createConsole(romfile)) {
      theOSystem->settings().setString("rom_file", romfile);
    } else {
      throw std::runtime_error("unknown error");
    }

    // seed random number generator
    if (theOSystem->settings().getString("random_seed") == "time") {
      srand((unsigned)time(0));
    } else {
      int seed = theOSystem->settings().getInt("random_seed");
      assert(seed >= 0);
      srand((unsigned)seed);
    }

    theOSystem->console().setPalette("standard");
}

void ALEInterface::getRGB(
    unsigned char pixel,
    unsigned char &red,
    unsigned char &green,
    unsigned char &blue
) {

    static const int ntsc_tbl[] = {
        0x000000, 0, 0x4a4a4a, 0, 0x6f6f6f, 0, 0x8e8e8e, 0,
        0xaaaaaa, 0, 0xc0c0c0, 0, 0xd6d6d6, 0, 0xececec, 0,
        0x484800, 0, 0x69690f, 0, 0x86861d, 0, 0xa2a22a, 0,
        0xbbbb35, 0, 0xd2d240, 0, 0xe8e84a, 0, 0xfcfc54, 0,
        0x7c2c00, 0, 0x904811, 0, 0xa26221, 0, 0xb47a30, 0,
        0xc3903d, 0, 0xd2a44a, 0, 0xdfb755, 0, 0xecc860, 0,
        0x901c00, 0, 0xa33915, 0, 0xb55328, 0, 0xc66c3a, 0,
        0xd5824a, 0, 0xe39759, 0, 0xf0aa67, 0, 0xfcbc74, 0,
        0x940000, 0, 0xa71a1a, 0, 0xb83232, 0, 0xc84848, 0,
        0xd65c5c, 0, 0xe46f6f, 0, 0xf08080, 0, 0xfc9090, 0,
        0x840064, 0, 0x97197a, 0, 0xa8308f, 0, 0xb846a2, 0,
        0xc659b3, 0, 0xd46cc3, 0, 0xe07cd2, 0, 0xec8ce0, 0,
        0x500084, 0, 0x68199a, 0, 0x7d30ad, 0, 0x9246c0, 0,
        0xa459d0, 0, 0xb56ce0, 0, 0xc57cee, 0, 0xd48cfc, 0,
        0x140090, 0, 0x331aa3, 0, 0x4e32b5, 0, 0x6848c6, 0,
        0x7f5cd5, 0, 0x956fe3, 0, 0xa980f0, 0, 0xbc90fc, 0,
        0x000094, 0, 0x181aa7, 0, 0x2d32b8, 0, 0x4248c8, 0,
        0x545cd6, 0, 0x656fe4, 0, 0x7580f0, 0, 0x8490fc, 0,
        0x001c88, 0, 0x183b9d, 0, 0x2d57b0, 0, 0x4272c2, 0,
        0x548ad2, 0, 0x65a0e1, 0, 0x75b5ef, 0, 0x84c8fc, 0,
        0x003064, 0, 0x185080, 0, 0x2d6d98, 0, 0x4288b0, 0,
        0x54a0c5, 0, 0x65b7d9, 0, 0x75cceb, 0, 0x84e0fc, 0,
        0x004030, 0, 0x18624e, 0, 0x2d8169, 0, 0x429e82, 0,
        0x54b899, 0, 0x65d1ae, 0, 0x75e7c2, 0, 0x84fcd4, 0,
        0x004400, 0, 0x1a661a, 0, 0x328432, 0, 0x48a048, 0,
        0x5cba5c, 0, 0x6fd26f, 0, 0x80e880, 0, 0x90fc90, 0,
        0x143c00, 0, 0x355f18, 0, 0x527e2d, 0, 0x6e9c42, 0,
        0x87b754, 0, 0x9ed065, 0, 0xb4e775, 0, 0xc8fc84, 0,
        0x303800, 0, 0x505916, 0, 0x6d762b, 0, 0x88923e, 0,
        0xa0ab4f, 0, 0xb7c25f, 0, 0xccd86e, 0, 0xe0ec7c, 0,
        0x482c00, 0, 0x694d14, 0, 0x866a26, 0, 0xa28638, 0,
        0xbb9f47, 0, 0xd2b656, 0, 0xe8cc63, 0, 0xfce070, 0
    };

    int c = ntsc_tbl[pixel];
    red   = c >> 16;
    green = (c >> 8) & 0xFF;
    blue  = c & 0xFF;
}


class ALEInterface::Impl {

    public:

        // create an ALEInterface. This routine is not threadsafe!
        Impl(const std::string &rom_file);
        ~Impl();

        // Resets the game
        void reset_game();

        // Indicates if the game has ended
        bool game_over() const;

        // Applies an action to the game and returns the reward. It is the user's responsibility
        // to check if the game has ended and reset when necessary - this method will keep pressing
        // buttons on the game over screen.
        reward_t act(Action action);

        // Returns the vector of legal actions.
        ActionVect getLegalActionSet();

        // Returns the vector of the minimal set of actions needed to play the game.
        ActionVect getMinimalActionSet();

        // Minimum possible instantaneous reward.
        reward_t minReward() const;

        // Maximum possible instantaneous reward.
        reward_t maxReward() const;

        // The remaining number of lives.
        int lives() const;

        // Returns the frame number since the loading of the ROM
        int getFrameNumber() const;

        // Returns the frame number since the start of the current episode
        int getEpisodeFrameNumber() const;

        // Sets the episodic frame limit
        void setMaxNumFrames(int newMax);

        // Returns the current game screen
        const ALEScreen &getScreen() const;

        // Writes a screen out to PNG
        bool screenToPNG(const std::string &filename);

        // Returns the current RAM content
        const ALERAM &getRAM() const;

        // Saves the state of the system
        void saveState();

        // Loads the state of the system
        bool loadState();

        // Gets current state as string
        std::string getSnapshot() const;

        // restores state from a string
        void restoreSnapshot(const std::string& snapshot);

        // accessors
        const OSystem &osystem() const;
        const Settings &settings() const;
        const RomSettings &romSettings() const;
        const StellaEnvironment &environment() const;

        /** Get the current version of the ALE interface.
            Major versions indicate significant changes that might break backward compatibility.
            Minor versions indicate bug-fixes. */
        void getVersion(int &major, int &minor) const;

    private:

        struct Emulator {

            Emulator() : osystem(NULL), settings(NULL), environment(NULL), runs(0) {}

            OSystem           *osystem;
            Settings          *settings;
            StellaEnvironment *environment;
            size_t            runs;
        };

        // Loads and initializes a game. After this call the game should be ready to play.
        void loadROM(const std::string &rom_file);

        std::auto_ptr<Emulator> m_emu;
        std::auto_ptr<RomSettings> m_rom_settings;

        reward_t m_episode_score; // Score accumulated throughout the course of an episode
        bool m_display_active;    // Should the screen be displayed or not
        int m_max_num_frames;     // Maximum number of frames for each episode
};


ALEInterface::Impl::~Impl() {

    if (m_emu->environment) delete m_emu->environment;
    if (m_emu->osystem)     delete m_emu->osystem;
    if (m_emu->settings)    delete m_emu->settings;
}


void ALEInterface::Impl::loadROM(const std::string &rom_file) {

    // build the ROM settings object
    m_rom_settings.reset(buildRomRLWrapper(rom_file));

    // now build the emulator 
    m_emu.reset(new ALEInterface::Impl::Emulator());

    int argc = 6;
    char** argv = new char*[argc];
    for (int i=0; i < argc; i++) {
        argv[i] = new char[200+rom_file.length()];
    }
    strcpy(argv[0],"./ale");
    strcpy(argv[1],"-player_agent");
    strcpy(argv[2],"random_agent");
    strcpy(argv[3],"-display_screen");
    if (m_display_active) strcpy(argv[4],"true");
    else strcpy(argv[4],"false");

    strcpy(argv[5],rom_file.c_str());
    createOSystem(argc, argv, m_emu->osystem, m_emu->settings);

    m_emu->osystem->settings().setBool("disable_color_averaging", true);
    m_emu->osystem->settings().setBool("backward_compatible_save", true);
    
    m_emu->environment = new StellaEnvironment(m_emu->osystem, m_rom_settings.get());
    m_max_num_frames = m_emu->osystem->settings().getInt("max_num_frames_per_episode");

    for (int i=0; i < argc; i++) {
        delete [] argv[i];
    }
    delete [] argv;

    // now ready the game to play
    reset_game();
}


bool ALEInterface::Impl::loadState() {

    return m_emu->environment->load();
}


bool ALEInterface::Impl::game_over() const {
    return (m_emu->environment->isTerminal() || (m_max_num_frames > 0 && getEpisodeFrameNumber() >= m_max_num_frames));
}


void ALEInterface::Impl::reset_game() {

    m_emu->environment->reset();
}


void ALEInterface::Impl::saveState() {
    m_emu->environment->save();
}

std::string ALEInterface::Impl::getSnapshot() const{

    const ALEState* state = m_emu->environment->cloneState();
    std::string snapshot = state->getStateAsString();
    m_emu->environment->destroyState(state);
    return snapshot;
}

void ALEInterface::Impl::restoreSnapshot(const std::string &snapshot) {

    ALEState state(snapshot);

    m_emu->environment->restoreState(state);
}


const ALERAM &ALEInterface::Impl::getRAM() const {
    return m_emu->environment->getRAM();
}


const ALEScreen &ALEInterface::Impl::getScreen() const {
    return m_emu->environment->getScreen();
}


void ALEInterface::Impl::setMaxNumFrames(int newMax) {
    m_max_num_frames = newMax;
}


int ALEInterface::Impl::getEpisodeFrameNumber() const {
    return m_emu->environment->getEpisodeFrameNumber();
}


int ALEInterface::Impl::getFrameNumber() const {
    return m_emu->environment->getFrameNumber();
}


ActionVect ALEInterface::Impl::getMinimalActionSet() {

    return m_rom_settings->getMinimalActionSet();
}


ActionVect ALEInterface::Impl::getLegalActionSet() {
    
    return m_rom_settings->getAllActions();
}


reward_t ALEInterface::Impl::minReward() const {

    return m_rom_settings->minReward();
}


reward_t ALEInterface::Impl::maxReward() const {

    return m_rom_settings->maxReward();
}


int ALEInterface::Impl::lives() const {

    return m_rom_settings->lives();
}


reward_t ALEInterface::Impl::act(Action action) {

    m_emu->environment->act(action, PLAYER_B_NOOP);
    reward_t reward = m_rom_settings->getReward();

    // sanity check rewards
    assert(reward <= m_rom_settings->maxReward());
    assert(reward >= m_rom_settings->minReward());

    if (m_display_active)
        m_emu->osystem->p_display_screen->display_screen(m_emu->osystem->console().mediaSource());

    return reward;
}


ALEInterface::Impl::Impl(const std::string &rom_file) :
    m_episode_score(0),
    m_display_active(false)
{
    loadROM(rom_file);
}


const OSystem &ALEInterface::Impl::osystem() const {

    return *m_emu->osystem;
}


const Settings &ALEInterface::Impl::settings() const {

    return *m_emu->settings;
}


const RomSettings &ALEInterface::Impl::romSettings() const {

    return *m_rom_settings;
}


const StellaEnvironment &ALEInterface::Impl::environment() const {

    return *m_emu->environment;
}


bool ALEInterface::Impl::screenToPNG(const std::string &filename) {

    // create a temporary display matrix
    IntMatrix buf;
    for (int i=0; i < getScreen().height(); i++) {
        buf.push_back(IntVect());
        for (int j=0; j < getScreen().width(); j++) {
            buf.back().push_back(getScreen().pixel(i,j));
        }
    }

    m_emu->osystem->p_export_screen->save_png(&buf, filename);

    return true;
}


void ALEInterface::Impl::getVersion(int &major, int &minor) const {

    major = ALEMajorVersion;
    minor = ALEMinorVersion;
}


/* --------------------------------------------------------------------------------------------------*/

/* begin PIMPL wrapper */

bool ALEInterface::loadState() {
    return m_pimpl->loadState();
}


bool ALEInterface::gameOver() const {
    return m_pimpl->game_over();
}


void ALEInterface::resetGame() {
    m_pimpl->reset_game();
}


void ALEInterface::saveState() {
    m_pimpl->saveState();
}


std::string ALEInterface::getSnapshot() const {
    return m_pimpl->getSnapshot();
}


void ALEInterface::restoreSnapshot(const std::string& snapshot) {
    m_pimpl->restoreSnapshot(snapshot);
}

const ALERAM &ALEInterface::getRAM() const {
    return m_pimpl->getRAM();
}


const ALEScreen &ALEInterface::getScreen() const {
    return m_pimpl->getScreen();
}


void ALEInterface::setMaxNumFrames(int newMax) {
    m_pimpl->setMaxNumFrames(newMax);
}


int ALEInterface::getEpisodeFrameNumber() const {
    return m_pimpl->getEpisodeFrameNumber();
}


int ALEInterface::getFrameNumber() const {
    return m_pimpl->getFrameNumber();
}


ActionVect ALEInterface::getMinimalActionSet() {
    return m_pimpl->getMinimalActionSet();
}


ActionVect ALEInterface::getLegalActionSet() {
    return m_pimpl->getLegalActionSet();
}


reward_t ALEInterface::minReward() const {
    return m_pimpl->minReward();
}


reward_t ALEInterface::maxReward() const {
    return m_pimpl->maxReward();
}


int ALEInterface::lives() const {
    return m_pimpl->lives();
}


reward_t ALEInterface::act(Action action) {
    return m_pimpl->act(action);
}


ALEInterface::ALEInterface(const std::string &rom_file) :
    m_pimpl(new ALEInterface::Impl(rom_file))
{
}


const OSystem &ALEInterface::osystem() const {

    return m_pimpl->osystem();
}


bool ALEInterface::screenToPNG(const std::string &filename) {

    return m_pimpl->screenToPNG(filename);
}


ALEInterface::~ALEInterface() {
    delete m_pimpl;
}


void ALEInterface::getVersion(int &major, int &minor) const {

    return m_pimpl->getVersion(major, minor);
}

const ALEScreen &ALEScreen::operator=(const ALEScreen &rhs) {

    // If array dimensions are mistmatched, must reallocate the array
    if (width() != rhs.width() || height() != rhs.height()) {
        m_width = rhs.width();
        m_height = rhs.height();

        m_pixels.resize(width() * height());
    }

    // Copy data over
    std::copy(rhs.getArray().begin(), rhs.getArray().end(), m_pixels.begin());

    return *this;
}


ALERAM::ALERAM() { }


ALERAM::ALERAM(const ALERAM &rhs) {
    // Copy data over
    memcpy(m_ram, rhs.m_ram, sizeof(m_ram));
}


ALERAM& ALERAM::operator=(const ALERAM &rhs) {

    if (this != &rhs)
        memcpy(m_ram, rhs.m_ram, sizeof(m_ram));

    return *this;
}


bool ALERAM::equals(const ALERAM &rhs) const {
  return memcmp(m_ram, rhs.m_ram, size()) == 0;
}


byte_t ALERAM::get(unsigned int x) const {

    // Wrap RAM around the first 128 bytes
    return m_ram[x & 0x7F];
}


byte_t* ALERAM::byte(unsigned int x) {
    return &m_ram[x & 0x7F];
}


} // namespace ale

