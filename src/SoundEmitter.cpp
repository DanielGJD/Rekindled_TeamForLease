/*****************************************************************************/
/*!
\file    SoundEmitter.cpp
\author  Jiangdi Gou
\par     contact: jiangdi.g@digipen.edu
\brief
Gettors/Settors for the sound
\remarks


All content 2015 DigiPen (USA) Corporation, all rights reserved.
*/
/*****************************************************************************/

#include "SoundEmitter.h"
#include "Engine.h"

namespace ForLeaseEngine{
	namespace Components{
SoundEmitter::SoundEmitter(Entity& Owner) : playSoundOnStart(true),Component(Owner, ComponentType::Transform)
{
	m_manager = ForLease->sound;
	if (playSoundOnStart)
		PlayEvent(startSound);
}

void SoundEmitter::Initialize()
{
	// * engine;


}

SoundEmitter::~SoundEmitter()
{
	if (playSoundOnStart)
		StopEvent(startSound);
}


void SoundEmitter::PlayEvent(std::string name)
{
    if (name.empty())
        return;
	ForLease->sound->PlayEvent(name);
}

void SoundEmitter::StopEvent(std::string name)
{
    if (name.empty())
        return;
    ForLease->sound->StopSound(name);
}
void SoundEmitter::SetPause(bool pause, std::string name)
{
    if (name.empty())
        return;
	if (pause == true)
        ForLease->sound->Pause(true,name);
	else
        ForLease->sound->Pause(false,name);
}

void SoundEmitter::BeQuiet()
{
    ForLease->sound->PauseAll();
    ForLease->sound->Update(0.0f);
}

void SoundEmitter::Rock()
{
    ForLease->sound->ResumeAll();
    ForLease->sound->Update(0.0f);
}

void SoundEmitter::SetVolume(float vol, std::string name)
{
    if (name.empty())
        return;
    ForLease->sound->Volume(vol, name);
}

void SoundEmitter::SetGlobal(float vol)
{
    ForLease->sound->SetGlobalVol(vol);
}

void SoundEmitter::SetGlobalVol(float vol)
{
    ForLease->sound->SetGlobalVolume(vol);
    ForLease->sound->Update(0.0f);
}

void SoundEmitter::Update()
{

}
void SoundEmitter::Release()
{

}

void SoundEmitter::Serialize(Serializer& root)
{
       root.WriteUint("Type", static_cast<unsigned int>(Type));
             Serializer soundEmitter = root.GetChild("SoundEmitter");
             soundEmitter.WriteUint("Type", static_cast<unsigned int>(Type));
             soundEmitter.WriteString("startSound", startSound);
             soundEmitter.WriteBool("playSoundOnStart", playSoundOnStart);
             //soundEmitter.WriteFloat("Pitch", Pitch);
             //soundEmitter.WriteFloat("Volume", Volume);
             //soundEmitter.WriteBool("Looping", Looping);
             //soundEmitter.WriteInt("LoopCount", LoopCount);
             root.Append(soundEmitter, "SoundEmitter");
}

void SoundEmitter::Deserialize(Serializer& root)
{
             Serializer soundEmitter = root.GetChild("SoundEmitter");
             soundEmitter.ReadString("startSound",startSound);
             soundEmitter.ReadBool("playSoundOnStart", playSoundOnStart);
//             soundEmitter.ReadFloat("Pitch", Pitch);
//             soundEmitter.ReadFloat("Volume", Volume);
//             soundEmitter.ReadBool("Looping", Looping);
//             soundEmitter.ReadInt("LoopCount", LoopCount);
}
// void SoundEmitter::SerializeRead(Serializer& str)
// {
  // StreamRead(str, playSoundOnStart);
  // StreamRead(str, startSound);
// }
// void SoundEmitter::SerializeWrite(Serializer& str)
// {
  // StreamWrite(str, playSoundOnStart);
  // StreamWrite(str);
  // StreamWrite(str, startSound);
  // StreamWrite(str);
// }
	}
}

