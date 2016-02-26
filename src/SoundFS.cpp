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
//#include "Precompiled.h"
#include "SoundFS.h"
#include <iostream>
//SoundManager * sound = nullptr;

namespace ForLeaseEngine{
	namespace Systems{
SoundManager::SoundManager() : /*ISystem(),*/ m_Sys(nullptr), m_MasterBank(nullptr), m_StringsBank(nullptr)
{
	//sound = this;
    FMOD_RESULT result_;
	// here we create our sound system
	char text[200];

	result_ = FMOD_Studio_System_Create(reinterpret_cast<FMOD_STUDIO_SYSTEM**>(&m_Sys), FMOD_VERSION);
	if (result_ != FMOD_OK)
	{
		std::cout << result_ << std::endl;
		printf("sound created?");
		//return;
	}
  //For later... check for errors

    result_ = FMOD_Studio_System_Initialize(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys),1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
	if ( result_ != FMOD_OK)
	{
	    std::cout<< result_ << std::endl;
		printf("Bank creation NOT initialized\n");
		//return;
	}



	return;
}

SoundManager::~SoundManager()
{

}


//    MessageBox(NULL, "ERROR: Could not create Sound System",
//      strcpy(text, strcat("Error: ", system_name)), NULL);
//
//    return false;
//  }

void SoundManager::Initialize(const char* PathFileName, const char * StringBank)
{
    FMOD_RESULT result_;
    result_ = FMOD_Studio_System_LoadBankFile(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys),PathFileName, FMOD_STUDIO_LOAD_BANK_NORMAL, reinterpret_cast<FMOD_STUDIO_BANK**>(&m_MasterBank));
	if (result_ != FMOD_OK)
	{
		std::cout << result_ << std::endl;
		printf("bankfile 1 is not loaded\n");
	}

//	result_ = m_Sys->loadBankFile("Ambience.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &m_AmbienceBank);
//	if (result_ != FMOD_OK)
//	{
//		std::cout << result_ << std::endl;
//		printf("banfile2 is not loaded\n");
//	}
//
//	result_ = m_Sys->loadBankFile("resources/Audio/RealBGM.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &m_MusicBank);
//	if (result_ != FMOD_OK)BalloonDetection
//	{
//		std::cout << result_ << std::endl;
//		printf("bankfile3 is not loaded\n");
//	}

	result_ = FMOD_Studio_System_LoadBankFile(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys),StringBank, FMOD_STUDIO_LOAD_BANK_NORMAL, reinterpret_cast<FMOD_STUDIO_BANK**>(&m_StringsBank));
	if (result_ != FMOD_OK)
	{
		std::cout << result_ << std::endl;
		printf("bankfile 4 is not loaded\n");
	}
}


void SoundManager::Update(float dt)
{
	FMOD_Studio_System_Update(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys));
}


void SoundManager::ShutDown()
{
	// we loop through the sounds list and call the built in release function
	for (Soundit iter = m_LoopSounds.begin(); iter != m_LoopSounds.end(); iter++)
	{
		FMOD_Studio_EventInstance_Release(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(iter->second));
	}
	// release the string bank;
	//m_StringsBank->unload();
	FMOD_Studio_Bank_Unload(reinterpret_cast<FMOD_STUDIO_BANK*>(m_StringsBank));
	//m_MasterBank->unload();
	FMOD_Studio_Bank_Unload(reinterpret_cast<FMOD_STUDIO_BANK*>(m_MasterBank));
	//
	//m_Sys->unloadAll();
	FMOD_Studio_System_UnloadAll(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys));
	//m_Sys->release();
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
	FMOD::Studio::EventDescription * SoundDescription = nullptr;
	FMOD::Studio::EventInstance * SoundInstance = nullptr;

	std::string temptext = "event:/" + name;


	// this is where we actually read in the GUIDs.txt and find the music's name
	FMOD_RESULT SoundResult = FMOD_Studio_System_GetEvent(reinterpret_cast<FMOD_STUDIO_SYSTEM*>(m_Sys),temptext.c_str(), reinterpret_cast<FMOD_STUDIO_EVENTDESCRIPTION**>(&SoundDescription));

	if (SoundResult == FMOD_ERR_EVENT_NOTFOUND)
	{
		return false;
	}

	//SoundDescription->createInstance(&SoundInstance);
	FMOD_Studio_EventDescription_CreateInstance(reinterpret_cast<FMOD_STUDIO_EVENTDESCRIPTION*>(SoundDescription), reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE**>(&SoundInstance));
	bool OneShot = false;
	//properties access
	//SoundDescription->isOneshot(&OneShot);
	FMOD_Studio_EventDescription_IsOneshot(reinterpret_cast<FMOD_STUDIO_EVENTDESCRIPTION *>(SoundDescription),reinterpret_cast<FMOD_BOOL*>(OneShot));

	//if we cant find the music file in the list
	if (!OneShot && m_LoopSounds.find(name) != m_LoopSounds.end())
	{
		return false;
	}

	// start the sound
	//SoundInstance->start();
	FMOD_Studio_EventInstance_Start(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*> (SoundInstance));

	if (OneShot)
		//SoundInstance->release();
		FMOD_Studio_EventInstance_Release(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(SoundInstance));
	else
		m_LoopSounds.insert(std::pair<std::string, FMOD::Studio::EventInstance *>(name, SoundInstance));

	return true;
}


bool SoundManager::StopSound(std::string name)
{
	if (m_LoopSounds.find(name) == m_LoopSounds.end())
		return true;
	//if (m_LoopSounds[name]->isValid())
	//if(FMOD_Studio_CueInstance_IsValid(reinterpret_cast<FMOD_STUDIO_CUEINSTANCE*>(m_LoopSounds[name])))
	if(FMOD_Studio_EventInstance_IsValid(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(m_LoopSounds[name])))
	{
		FMOD_STUDIO_PLAYBACK_STATE rock = FMOD_STUDIO_PLAYBACK_PLAYING;
		//m_LoopSounds[name]->getPlaybackState(&rock);
		FMOD_Studio_EventInstance_GetPlaybackState(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE *>(m_LoopSounds[name]),reinterpret_cast<FMOD_STUDIO_PLAYBACK_STATE *>(rock));

		//m_LoopSounds[name]->setPaused(true);
		FMOD_Studio_EventInstance_SetPaused(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(m_LoopSounds[name]), (true));
		 // Don't neet to pause before stopping.
		//m_LoopSounds[name]->stop(FMOD_STUDIO_STOP_IMMEDIATE);
		FMOD_Studio_EventInstance_Stop(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE *>(m_LoopSounds[name]),FMOD_STUDIO_STOP_IMMEDIATE);

		//m_LoopSounds[name]->getPlaybackState(&rock);
		FMOD_Studio_EventInstance_GetPlaybackState(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE *>(m_LoopSounds[name]),reinterpret_cast<FMOD_STUDIO_PLAYBACK_STATE *>(rock));

		//m_LoopSounds[name]->release();
		FMOD_Studio_EventInstance_Release(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(m_LoopSounds[name]));
	}


    m_LoopSounds.erase(name);
	return true;
}


void SoundManager::Pause(bool pauseSound, std::string name)
{
	//m_LoopSounds[name]->setPaused(pauseSound);
	FMOD_Studio_EventInstance_SetPaused(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(m_LoopSounds[name]), (pauseSound));
}


void SoundManager::PauseAll()
{
	for (Soundit it = m_LoopSounds.begin(); it != m_LoopSounds.end(); ++it)
	{
		//FMOD_RESULT myfuckingresult = it->second->setPaused(true);
		FMOD_Studio_EventInstance_SetPaused(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(it->second), (true));
		//int randonum = 0;
	}
}

void SoundManager::StopAll()
{
	for (Soundit it = m_LoopSounds.begin(); it != m_LoopSounds.end(); ++it)
	{
		//FMOD_RESULT myfuckingresult = it->second->setPaused(true);
		FMOD_Studio_EventInstance_SetPaused(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(it->second), (true));
		//int randonum = 0;
	}
}

void SoundManager::ResumeAll()
{
	for (Soundit it = m_LoopSounds.begin(); it != m_LoopSounds.end(); ++it)
	{
		//FMOD_RESULT myfuckingresult = it->second->setPaused(false);
		FMOD_Studio_EventInstance_SetPaused(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*>(it->second), (false));
		//int randonumshit = 9;
	}
}


void SoundManager::Volume(float vol, std::string name)
{
	//m_LoopSounds[name]->setVolume(vol);
	FMOD_Studio_EventInstance_SetVolume(reinterpret_cast<FMOD_STUDIO_EVENTINSTANCE*> (m_LoopSounds[name]),vol);
}

 }
}
