#include "Sound.h"

namespace ForLeaseEngine {
    Sound::Sound(std::string filename, FMOD_SOUND* soundSource) : FileName(filename), SoundSource(soundSource) {}

    Sound::~Sound() {
        FMOD_Sound_Release(SoundSource);
    }

    std::string Sound::GetFileName() {
        return FileName;
    }

    FMOD_SOUND** Sound::GetSoundSource() {
        return &SoundSource;
    }
}
