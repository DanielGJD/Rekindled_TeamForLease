/*****************************************************************************/
/*!
\file    SoundFS.cpp
\author  Jiangdi Gou
\par     contact: jiangdi.g@digipen.edu
\brief
the implementation for the base sound load/play functions using FMOD studio
with bank file supported.
\remarks


All content 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "SoundFS.h"
#include <iostream>

namespace ForLeaseEngine{
    namespace Systems{
        SoundManager::SoundManager() : /*ISystem(),*/ m_Sys(nullptr), m_MasterBank(nullptr), m_StringsBank(nullptr)
        {
            FMOD_RESULT result_;
            char text[200];

            result_ = FMOD_Studio_System_Create(reinterpret_cast<FMOD_STUDIO_SYSTEM**>(&m_Sys), FMOD_VERSION);

            result_ = FMOD_Studio_System_Initialize(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys),1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);

            Initialize("sounds/Master\ Bank.bank", "sounds/Master\ Bank.strings.bank");
        }

        SoundManager::~SoundManager()
        {

        }

        void SoundManager::Initialize(const char* PathFileName, const char * StringBank)
        {
            FMOD_RESULT result_;
            result_ = FMOD_Studio_System_LoadBankFile(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys),PathFileName, FMOD_STUDIO_LOAD_BANK_NORMAL, reinterpret_cast<FMOD_STUDIO_BANK**>(&m_MasterBank));

            result_ = FMOD_Studio_System_LoadBankFile(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys),StringBank, FMOD_STUDIO_LOAD_BANK_NORMAL, reinterpret_cast<FMOD_STUDIO_BANK**>(&m_StringsBank));

            FMOD_Studio_System_GetBus(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), "Background", reinterpret_cast<FMOD_STUDIO_BUS**>(&m_Background));
            FMOD_Studio_System_GetBus(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), "Effects", reinterpret_cast<FMOD_STUDIO_BUS**>(&m_Effects));
        }


        void SoundManager::Update(float dt)
        {
            FMOD_Studio_System_Update(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys));
        }


        void SoundManager::ShutDown()
        {
            // we loop through the sounds list and call the built in release function
            //for (Soundit iter = m_LoopSounds.begin(); iter != m_LoopSounds.end(); iter++)
            //{
            //    FMOD_Studio_EventInstance_Release(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(iter->second));
            //}
            FMOD_Studio_Bank_Unload(reinterpret_cast<FMOD_STUDIO_BANK*>(m_StringsBank));
            FMOD_Studio_Bank_Unload(reinterpret_cast<FMOD_STUDIO_BANK*>(m_MasterBank));
            FMOD_Studio_System_UnloadAll(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys));
            FMOD_Studio_System_Release(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys));
        }

        std::vector<std::string>SoundManager::GetName()
        {
            // Just to make sure, this function is to provide level
            // editor a list of sound file's name generated in GUIDS.txt
            // Not the actual function to read GUIDs.txt
            std::string temptext;
            std::vector<std::string> SoundsList;
            std::ifstream readfile("sounds/GUIDs.txt");

            while(std::getline(readfile,temptext))
            {
                std::string key = "event:/";
                unsigned pos = temptext.find(key);

                if(pos != std::string::npos)
                {
                    pos += key.length();
                    SoundsList.push_back(temptext.substr(pos));
                }
            }

            return SoundsList;
        }

        bool SoundManager::PlayEvent(std::string name)
        {
            //FMOD::Studio::EventDescription * SoundDescription = nullptr;
            //FMOD::Studio::EventInstance * SoundInstance = nullptr;

            //std::string temptext = "event:/" + name;


            //// this is where we actually read in the GUIDs.txt and find the music's name
            //FMOD_RESULT SoundResult = FMOD_Studio_System_GetEvent(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys),temptext.c_str(), reinterpret_cast<FMOD_STUDIO_EVENTDESCRIPTION**>(&SoundDescription));

            //if (SoundResult == FMOD_ERR_EVENT_NOTFOUND)
            //{
            //    return false;
            //}

            //FMOD_Studio_EventDescription_CreateInstance(reinterpret_cast<FMOD_STUDIO_EVENTDESCRIPTION*>(SoundDescription), reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE**>(&SoundInstance));
            //bool OneShot = false;
            ////properties access
            //FMOD_Studio_EventDescription_IsOneshot(reinterpret_cast<FMOD_STUDIO_EVENTDESCRIPTION *>(SoundDescription),reinterpret_cast<FMOD_BOOL*>(OneShot));

            ////if we cant find the music file in the list
            //if (!OneShot && m_LoopSounds.find(name) != m_LoopSounds.end())
            //{
            //    return false;
            //}

            //// start the sound
            ////SoundInstance->start();
            //FMOD_Studio_EventInstance_Start(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*> (SoundInstance));

            //if (OneShot)
            //    FMOD_Studio_EventInstance_Release(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(SoundInstance));
            //else
            //    m_LoopSounds.insert(std::pair<std::string, FMOD::Studio::EventInstance *>(name, SoundInstance));

            //return true;

            FMOD_STUDIO_EVENTDESCRIPTION* eventDescription = 0;
            FMOD_STUDIO_EVENTINSTANCE* event = 0;
            FMOD_Studio_System_GetEvent(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), name.c_str(), &eventDescription);
            FMOD_Studio_EventDescription_CreateInstance(reinterpret_cast<FMOD_STUDIO_EVENTDESCRIPTION*>(eventDescription), &event);
            FMOD_Studio_EventInstance_Start(event);
        }


        bool SoundManager::StopSound(std::string name)
        {
            //if (m_LoopSounds.find(name) == m_LoopSounds.end())
            //    return true;
            ////if (m_LoopSounds[name]->isValid())
            ////if(FMOD_Studio_CueInstance_IsValid(reinterpret_cast<FMOD_STUDIO_CUEINSTANCE*>(m_LoopSounds[name])))
            //if(FMOD_Studio_EventInstance_IsValid(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(m_LoopSounds[name])))
            //{
            //    FMOD_STUDIO_PLAYBACK_STATE rock = FMOD_STUDIO_PLAYBACK_PLAYING;
            //    FMOD_Studio_EventInstance_GetPlaybackState(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE *>(m_LoopSounds[name]),reinterpret_cast<FMOD_STUDIO_PLAYBACK_STATE *>(rock));
            //    FMOD_Studio_EventInstance_SetPaused(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(m_LoopSounds[name]), (true));
            //    FMOD_Studio_EventInstance_Stop(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE *>(m_LoopSounds[name]),FMOD_STUDIO_STOP_IMMEDIATE);
            //    FMOD_Studio_EventInstance_GetPlaybackState(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE *>(m_LoopSounds[name]),reinterpret_cast<FMOD_STUDIO_PLAYBACK_STATE *>(rock));
            //    FMOD_Studio_EventInstance_Release(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(m_LoopSounds[name]));
            //}


            //m_LoopSounds.erase(name);
            //return true;

            //FMOD_STUDIO_EVENTINSTANCE* eventDescription;
            //FMOD_Studio_System_GetEvent(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), name.c_str(), NULL);
            //FMOD_Studio_EventInstance_Stop(event, FMOD_STUDIO_STOP_IMMEDIATE);

            FMOD_STUDIO_EVENTDESCRIPTION* eventDescription = 0;
            FMOD_STUDIO_EVENTINSTANCE* event = 0;
            if (FMOD_Studio_EventInstance_IsValid())
            FMOD_Studio_System_GetEvent(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), name.c_str(), &eventDescription);
            FMOD_Studio_EventDescription_CreateInstance(reinterpret_cast<FMOD_STUDIO_EVENTDESCRIPTION*>(eventDescription), &event);
            FMOD_Studio_EventInstance_Stop(event, FMOD_STUDIO_STOP_IMMEDIATE);
        }


        void SoundManager::Pause(std::string name)
        {
            FMOD_STUDIO_EVENTINSTANCE* event;
            FMOD_Studio_System_GetEvent(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), name.c_str(), NULL);
            FMOD_Studio_EventInstance_SetPaused(event, true);
        }

        void SoundManager::Resume(std::string name)
        {
            FMOD_STUDIO_EVENTINSTANCE* event;
            FMOD_Studio_System_GetEvent(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), name.c_str(), NULL);
            FMOD_Studio_EventInstance_SetPaused(event, false);
        }


        void SoundManager::PauseGlobal()
        {
            PauseBackground();
            PauseEffects();
        }

        void SoundManager::ResumeGlobal()
        {
            ResumeBackground();
            ResumeEffects();
        }

        void SoundManager::PauseBackground() {
            FMOD_Studio_Bus_SetPaused(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Background), true);
        }

        void SoundManager::ResumeBackground() {
            FMOD_Studio_Bus_SetPaused(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Background), false);
        }

        void SoundManager::PauseEffects() {
            FMOD_Studio_Bus_SetPaused(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Effects), true);
        }

        void SoundManager::ResumeEffects() {
            FMOD_Studio_Bus_SetPaused(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Effects), false);
        }

        void SoundManager::Volume(float volume, std::string name)
        {
            FMOD_STUDIO_EVENTINSTANCE* event = 0;
            FMOD_Studio_System_GetEvent(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), name.c_str(), &event);
            FMOD_Studio_EventInstance_SetVolume(event, volume);
        }

        void SoundManager::SetGlobalVolume(float volume)
        {
            SetBackgroundVolume(volume);
            SetEffectsVolume(volume);
        }

        void SoundManager::SetBackgroundVolume(float volume) {
            FMOD_Studio_Bus_SetFaderLevel(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Background), volume);
        }

        void SoundManager::SetEffectsVolume(float volume) {
            FMOD_Studio_Bus_SetFaderLevel(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Effects), volume);
        }

        void SoundManager::MuteGlobal() {
            MuteBackground();
            MuteEffects();
        }

        void SoundManager::MuteBackground() {
            FMOD_Studio_Bus_SetMute(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Background), true);
        }

        void SoundManager::MuteEffects() {
            FMOD_Studio_Bus_SetMute(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Effects), true);
        }

        void SoundManager::UnmuteGlobal() {
            UnmuteBackground();
            UnmuteEffects();
        }

        void SoundManager::UnmuteBackground() {
            FMOD_Studio_Bus_SetMute(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Background), false);
        }

        void SoundManager::UnmuteEffects() {
            FMOD_Studio_Bus_SetMute(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Background), false);
        }
    }
}
