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

//#include "SoundEmitter.h"


//#pragma  comment (lib, "fmodstudio_vc")




namespace ForLeaseEngine{
	namespace Systems{

typedef FMOD::Studio::System * SoundSys;
typedef FMOD::Studio::Bank * SoundBank;
typedef std::unordered_map<std::string, FMOD::Studio::EventInstance *> SoundCollection;
typedef std::unordered_map<std::string, FMOD::Studio::EventInstance *>::iterator Soundit;

class SoundManager //: public ISystem
{
	public:
		SoundManager();
		~SoundManager();


		void Initialize(const char* PathFileName);
		void Update(float dt);
		void ShutDown();

		bool PlayEvent(std::string name);
		bool StopSound(std::string name);
		void Pause(bool pause, std::string name);
		void PauseAll();
		void StopAll(void);
		void ResumeAll(void);
		void Resume(void);
		void Volume(float vol, std::string name);
		std::vector<std::string>GetName();

	private:
		SoundSys m_Sys;
		SoundBank m_AmbienceBank;
		SoundBank m_MusicBank;
		SoundBank m_MasterBank;
		SoundBank m_StringsBank;
		SoundCollection m_LoopSounds;
};

extern SoundManager * sound;

	}
}

#endif
