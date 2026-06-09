#include "Audio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <sys/stat.h>
#include <iostream>

static ma_engine engine;
static bool engineReady = false;
static ma_sound bgSound;
static bool bgReady = false;

static bool fileExists(const std::string& p) {
    struct stat st;
    return stat(p.c_str(), &st) == 0;
}

void Audio::init() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        std::cerr << "Audio: failed to initialize miniaudio engine\n";
        return;
    }
    engineReady = true;
}

void Audio::playOneShot(const std::string& path) {
    if (!engineReady) return;
    if (!fileExists(path)) return; // silently skip missing assets (e.g. gun.mp3)
    ma_engine_play_sound(&engine, path.c_str(), NULL);
}

void Audio::playLoop(const std::string& path) {
    if (!engineReady || bgReady) return;
    if (!fileExists(path)) {
        std::cerr << "Audio: '" << path << "' not found; skipping background music\n";
        return;
    }
    if (ma_sound_init_from_file(&engine, path.c_str(), MA_SOUND_FLAG_STREAM,
                                NULL, NULL, &bgSound) != MA_SUCCESS) {
        std::cerr << "Audio: failed to load '" << path << "'\n";
        return;
    }
    ma_sound_set_looping(&bgSound, MA_TRUE);
    ma_sound_start(&bgSound);
    bgReady = true;
}

void Audio::shutdown() {
    if (bgReady) ma_sound_uninit(&bgSound);
    if (engineReady) ma_engine_uninit(&engine);
    bgReady = false;
    engineReady = false;
}
