// /*!
    // \file   Audio.h
    // \author Christopher Hudson

    // \brief
        // Defines the audio system, mainly a wrapper around low level fmod

    // \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
// */

// #ifndef AUDIO_H
// #define AUDIO_H

// #include "fmod.h"
// #include "fmod_common.h"
// #include "fmod.hpp"
// #include "fmod_errors.h"
// #include "Sound.h"

// namespace ForLeaseEngine {
    // namespace Systems {
        // class Audio {
            // public:
                // Audio(int maxChannels = 32);
                // ~Audio();

                // void Update();

                // Sound* CreateSound(std::string const& filename);
                // FMOD_CHANNEL* PlayAudio(Sound* sound, bool looping = false, int loopCount = -1, float volume = 1, float pitch = 1);
                // FMOD_CHANNEL* PlayAudio(std::string const& filename, bool looping = false, int loopCount = -1, float volume = 1, float pitch = 1);
            // private:
                // FMOD_SYSTEM* AudioSystem;
                // float GlobalVolume;
                // float GlobalPitch;
        // };
    // }
// }

// #endif // AUDIO_H
