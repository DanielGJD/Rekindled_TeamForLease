/*!
    \file   Audio.cpp
    \author Christopher Hudson

    \brief
        Defines the audio system, mainly a wrapper around low level fmod

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Audio.h"
#include "Exception.h"
#include "Engine.h"
#include <iostream>

namespace ForLeaseEngine {
    namespace Systems {
        Audio::Audio(int maxChannels) {
            FMOD_RESULT errorCode;

            errorCode = FMOD_System_Create(&AudioSystem);
            if(errorCode != FMOD_OK) {
                throw new Exception(FMOD_ErrorString(errorCode));
            }

            errorCode = FMOD_System_Init(AudioSystem, maxChannels, FMOD_INIT_NORMAL, 0);
            GlobalVolume = 1;
            GlobalPitch = 1;
        }

        Audio::~Audio(){
            FMOD_RESULT errorCode;
            errorCode = FMOD_System_Release(AudioSystem);
            if(errorCode != FMOD_OK) {
                throw new Exception(FMOD_ErrorString(errorCode));
            }
        }

        void Audio::Update() {
            FMOD_System_Update(AudioSystem);
        }

        Sound* Audio::CreateSound(std::string const& filename) {
            FMOD_SOUND* sound;
            FMOD_RESULT errorCode = FMOD_System_CreateSound(AudioSystem, filename.c_str(), FMOD_DEFAULT, 0, &sound);

            if(errorCode != FMOD_OK) {
                std::cout << FMOD_ErrorString(errorCode);
                return NULL;
            }

            return new Sound(filename, sound);
        }

        FMOD_CHANNEL* Audio::PlayAudio(Sound* sound, bool looping, int loopCount, float volume, float pitch) {
            FMOD_CHANNEL* channel;
            FMOD_RESULT errorCode;

            if(looping) {
                FMOD_Sound_SetMode(*sound->GetSoundSource(), FMOD_LOOP_NORMAL | FMOD_2D);
                FMOD_Sound_SetLoopCount(*sound->GetSoundSource(), loopCount);
            }
            else {
                FMOD_Sound_SetMode(*sound->GetSoundSource(), FMOD_LOOP_OFF | FMOD_2D);
            }

            errorCode = FMOD_System_PlaySound(AudioSystem, *sound->GetSoundSource(), 0, false, &channel);
            if(errorCode != FMOD_OK) {
                return NULL;
            }

            FMOD_Channel_SetPitch(channel, pitch * GlobalPitch);
            FMOD_Channel_SetVolume(channel, volume * GlobalVolume);

            return channel;
        }

        FMOD_CHANNEL* Audio::PlayAudio(std::string const& filename, bool looping, int loopCount, float volume, float pitch) {
            Sound* sound = ForLease->Resources.GetSound(filename);
            if(!sound) {
                return NULL;
            }

            return PlayAudio(sound, looping, loopCount, volume, pitch);
        }
    }
}
