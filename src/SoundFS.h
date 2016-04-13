/*****************************************************************************/
/*!
\file    SoundFS.h
\author  Jiangdi Gou
\par     contact: jiangdi.g@digipen.edu
\brief
the header file for the base sound manager class.
\remarks


All content 2015 DigiPen (USA) Corporation, all rights reserved.
*/
/*****************************************************************************/

#ifndef SOUNDFS_H
#define SOUNDFS_H

#include"fmod_studio.hpp"
#include"fmod_common.h"
#include"fmod_errors.h"
#include"common.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>

namespace ForLeaseEngine{
    namespace Systems{

        typedef FMOD::Studio::System * SoundSys;
        typedef FMOD::System * LowLevelSoundSys;
        typedef FMOD::Studio::Bank * SoundBank;
        typedef std::unordered_map<std::string, FMOD::Studio::EventInstance *> SoundCollection;
        typedef std::unordered_map<std::string, FMOD::Studio::EventInstance *>::iterator Soundit;
        typedef FMOD_GUID * EventID;
        typedef FMOD::Studio::Bus * Buslist;

        class SoundManager //: public ISystem
        {
            public:
                SoundManager();
                ~SoundManager();


                void Initialize(const char* PathFileName, const char *StringBank);
                void Update(float dt);
                void ShutDown();

                // Play/pause controls
                bool PlayEvent(std::string name);
                bool StopSound(std::string name);
                void Pause(std::string name);
                void Resume(std::string name);
                void PauseGlobal();
                void ResumeGlobal();
                void PauseBackground();
                void ResumeBackground();
                void PauseEffects();
                void ResumeEffects();

                // Volume controls
                void Volume(float vol, std::string name);
                void SetGlobalVolume(float volume);
                void SetBackgroundVolume(float volume);
                void SetEffectsVolume(float volume);

                void MuteGlobal();
                void MuteBackground();
                void MuteEffects();
                void UnmuteGlobal();
                void UnmuteBackground();
                void UnmuteEffects();

                // Get list of all names
                std::vector<std::string>GetName();

            private:
                FMOD::Studio::System* m_Sys;
                FMOD::Studio::Bank* m_MasterBank;
                FMOD::Studio::Bank* m_StringsBank;
                //SoundCollection m_LoopSounds;
                //FMOD_GUID m_EventID;
                FMOD::Studio::Bus* m_Background;
                FMOD::Studio::Bus* m_Effects;
        };

    }
}

#endif
