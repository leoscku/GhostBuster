#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <string>

// Tiny audio facade backed by miniaudio (arm64-native, single header).
// Replaces the original FMOD integration, whose bundled dylib was x86_64-only.
namespace Audio {
    void init();
    void shutdown();
    void playOneShot(const std::string& path); // fire-and-forget; no-op if file missing
    void playLoop(const std::string& path);     // looping background track
}

#endif
