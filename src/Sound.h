/*!
    \file   Sound.h
    \author Christopher Hudson

    \brief
        Defines a class containing information on a loaded sound

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef SOUND_H
#define SOUND_H

#include "fmod.hpp"
#include <string>

namespace ForLeaseEngine {
    class Sound {
        public:
            Sound(std::string filename, FMOD_SOUND* soundSource);
            ~Sound();

            std::string GetFileName();
            FMOD_SOUND** GetSoundSource();
        private:
            std::string FileName;
            FMOD_SOUND* SoundSource;
    };
}

#endif // SOUND_H
