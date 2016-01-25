/*****************************************************************************/
/*!
\file    SoundEmitter.h
\author  Jiangdi Gou
\par     contact: jiangdi.g@digipen.edu
\brief
the header file for the sound emitter class.
\remarks


All content 2015 DigiPen (USA) Corporation, all rights reserved.
*/
/*****************************************************************************/
#ifndef SOUNDEMITTER_H
#define SOUNDEMITTER_H

#include "SoundFS.h"
#include "Component.h"
//#include "Audio.h"
#include "Serializable.h"
#include "Serialize.h"
#include <unordered_set>
#include <string>


//using namespace Zilch;
namespace ForLeaseEngine{
	namespace Components{
class SoundEmitter;

class SoundEmitter : public Component
{
public:
    static const ComponentType Type = ComponentType::SoundEmitter;
	virtual ComponentType GetType() { return Type; }
	//SoundEmitter() {};
	SoundEmitter(Entity& Owner);
	~SoundEmitter();

	void Initialize();
	void Update();
	void Release();

	void PlayEvent(std::string name);
	void StopEvent(std::string name);
	void BeQuiet();
	void Rock();
	void SetPause(bool pause, std::string name);
	void SetVolume(float vol, std::string name);
	void Serialize(Serializer& root);
    void Deserialize(Serializer& root);

  // void SerializeRead(Serializer& str) override;
  // void SerializeWrite(Serializer& str) override;

	//Set in editor (optional, see segment in FactoryManager)
	bool playSoundOnStart;
	std::string startSound;
private:
	// Core
	Systems::SoundManager* m_manager;


};
	}

}

#endif
