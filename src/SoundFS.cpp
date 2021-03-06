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

            if (result_ != FMOD_OK) {
                std::cout << result_ << std::endl;
                std::cout << "Failed to load master bankfile." << std::endl;
            }

            result_ = FMOD_Studio_System_LoadBankFile(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys),StringBank, FMOD_STUDIO_LOAD_BANK_NORMAL, reinterpret_cast<FMOD_STUDIO_BANK**>(&m_StringsBank));


            if (result_ != FMOD_OK) {
                std::cout << result_ << std::endl;
                std::cout << "Failed to load master strings file." << std::endl;
            }

            result_ = FMOD_Studio_System_GetBus(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), "bus:/Background", reinterpret_cast<FMOD_STUDIO_BUS**>(&m_Background));


            if (result_ != FMOD_OK) {
                std::cout << result_ << std::endl;
                std::cout << "Failed to get background bus." << std::endl;
            }

            result_ = FMOD_Studio_System_GetBus(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), "bus:/Effects", reinterpret_cast<FMOD_STUDIO_BUS**>(&m_Effects));


            if (result_ != FMOD_OK) {
                std::cout << result_ << std::endl;
                std::cout << "Failed to get effects bus." << std::endl;
            }

            result_ = FMOD_Studio_System_GetBus(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), "bus:/Menus", reinterpret_cast<FMOD_STUDIO_BUS**>(&m_Menus));


            if (result_ != FMOD_OK) {
                std::cout << result_ << std::endl;
                std::cout << "Failed to get menus bus." << std::endl;
            }
        }


        void SoundManager::Update()
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
            std::string eventName = "event:/" + name;

            std::cout << eventName << std::endl;

            FMOD_STUDIO_EVENTDESCRIPTION* eventDescription = 0;
            FMOD_STUDIO_EVENTINSTANCE* event = 0;
            FMOD_RESULT result = FMOD_Studio_System_GetEvent(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), eventName.c_str(), &eventDescription);

            if (result != FMOD_OK) {
                std::cout << result << std::endl;
                std::cout << "NOT OK" << std::endl;
                return false;
            }

            result = FMOD_Studio_EventDescription_CreateInstance(reinterpret_cast<FMOD_STUDIO_EVENTDESCRIPTION*>(eventDescription), &event);
            if (result != FMOD_OK) {
                std::cout << result << std::endl;
                std::cout << "NOT OK" << std::endl;
                return false;
            }
            FMOD_BOOL oneShot = false;
            FMOD_Studio_EventDescription_IsOneshot(eventDescription, &oneShot);

            result = FMOD_Studio_EventInstance_Start(event);

            if (result != FMOD_OK) {
                std::cout << result << std::endl;
                std::cout << "NOT OK" << std::endl;
                return false;
            }

            if (oneShot) {
                FMOD_Studio_EventInstance_Release(event);
            } else {
                m_Sounds.insert(std::pair<std::string, FMOD_STUDIO_EVENTINSTANCE *>(name, event));
            }

            return true;
        }


        bool SoundManager::StopSound(std::string name)
        {
            auto eventFind = m_Sounds.find(name);
            if (eventFind != m_Sounds.end()) {
                FMOD_Studio_EventInstance_Stop(eventFind->second, FMOD_STUDIO_STOP_IMMEDIATE);
                FMOD_Studio_EventInstance_Release(eventFind->second);
                m_Sounds.erase(eventFind->first);
            }

            return true;
        }


        void SoundManager::Pause(std::string name)
        {
            auto eventFind = m_Sounds.find(name);
            if (eventFind != m_Sounds.end()) {
                FMOD_Studio_EventInstance_SetPaused(eventFind->second, true);
            }
        }

        void SoundManager::Resume(std::string name)
        {
            auto eventFind = m_Sounds.find(name);
            if (eventFind != m_Sounds.end()) {
                FMOD_Studio_EventInstance_SetPaused(eventFind->second, false);
            }
        }


        void SoundManager::PauseGlobal()
        {
            PauseGameplay();
            PauseMenus();
        }

        void SoundManager::ResumeGlobal()
        {
            ResumeGameplay();
            ResumeMenus();
        }

        void SoundManager::PauseGameplay()
        {
            PauseBackground();
            PauseEffects();
        }

        void SoundManager::ResumeGameplay()
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

        void SoundManager::PauseMenus() {
            FMOD_Studio_Bus_SetPaused(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Menus), true);
        }

        void SoundManager::ResumeMenus() {
            FMOD_Studio_Bus_SetPaused(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Menus), false);
        }

        void SoundManager::Volume(float volume, std::string name)
        {
            //FMOD_STUDIO_EVENTINSTANCE* event = 0;
            //FMOD_Studio_System_GetEvent(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys), name.c_str(), &event);
            //FMOD_Studio_EventInstance_SetVolume(event, volume);

            auto eventFind = m_Sounds.find(name);
            if (eventFind != m_Sounds.end()) {
                FMOD_Studio_EventInstance_SetVolume(eventFind->second, volume);
            }
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

        void SoundManager::SetMenusVolume(float volume) {
            FMOD_Studio_Bus_SetFaderLevel(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Menus), volume);
        }

        void SoundManager::MuteGlobal() {
            MuteGameplay();
            MuteMenus();
        }

        void SoundManager::UnmuteGlobal() {
            UnmuteGameplay();
            UnmuteMenus();
        }

        void SoundManager::MuteGameplay() {
            MuteBackground();
            MuteEffects();
        }

        void SoundManager::UnmuteGameplay() {
            UnmuteBackground();
            UnmuteEffects();
        }

        void SoundManager::MuteBackground() {
            FMOD_Studio_Bus_SetMute(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Background), true);
        }

        void SoundManager::UnmuteBackground() {
            FMOD_Studio_Bus_SetMute(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Background), false);
        }

        void SoundManager::MuteEffects() {
            FMOD_Studio_Bus_SetMute(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Effects), true);
        }

        void SoundManager::UnmuteEffects() {
            FMOD_Studio_Bus_SetMute(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Effects), false);
        }

        void SoundManager::MuteMenus() {
            FMOD_Studio_Bus_SetMute(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Menus), true);
        }

        void SoundManager::UnmuteMenus() {
            FMOD_Studio_Bus_SetMute(reinterpret_cast<FMOD_STUDIO_BUS*>(m_Menus), false);
        }

        SoundManager::CurrentMuting SoundManager::GetMuting() {
            FMOD_BOOL bgMute = false;
            FMOD_BOOL fxMute = false;

            FMOD_Studio_Bus_GetMute(reinterpret_cast<FMOD_STUDIO_BUS *>(m_Background), &bgMute);
            FMOD_Studio_Bus_GetMute(reinterpret_cast<FMOD_STUDIO_BUS *>(m_Effects), &fxMute);

            CurrentMuting currentMuting;
            currentMuting.BackgroundMuted = bgMute;
            currentMuting.EffectsMuted = fxMute;

            return currentMuting;
        }
    }
}
