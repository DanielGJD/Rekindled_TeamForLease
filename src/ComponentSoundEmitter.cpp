/*!
    \file   ComponentSoundEmitter.cpp
    \author Christopher Hudson

    \brief
        Defines a component to allow emitting of sounds

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentSoundEmitter.h"
#include "Engine.h"

namespace ForLeaseEngine {
    namespace Components {
        SoundEmitter::SoundEmitter(Entity& owner)
                                  : Component(owner, ComponentType::Transform),
                                    Channel(NULL), Pitch(1), Volume(1), Looping(false), LoopCount(-1) {}

        void SoundEmitter::Update() {
        }

        void SoundEmitter::Serialize(Serializer& root) {
            Serializer soundEmitter = root.GetChild("SoundEmitter");
            soundEmitter.WriteFloat("Pitch", Pitch);
            soundEmitter.WriteFloat("Volume", Volume);
            soundEmitter.WriteBool("Looping", Looping);
            soundEmitter.WriteInt("LoopCount", LoopCount);
            root.Append(soundEmitter, "SoundEmitter");
        }

        void SoundEmitter::Deserialize(Serializer& root) {
            Serializer soundEmitter = root.GetChild("SoundEmitter");
            soundEmitter.ReadFloat("Pitch", Pitch);
            soundEmitter.ReadFloat("Volume", Volume);
            soundEmitter.ReadBool("Looping", Looping);
            soundEmitter.ReadInt("LoopCount", LoopCount);
        }

        void SoundEmitter::Play() {
            if(CurrentSound.length() != 0) {
                Stop();
                Channel = ForLease->AudioSystem->PlayAudio(CurrentSound, Looping, LoopCount, Volume, Pitch);
            }
        }

        void SoundEmitter::Play(std::string const& filename) {
            Stop();
            CurrentSound = filename;
            Channel = ForLease->AudioSystem->PlayAudio(CurrentSound, Looping, LoopCount, Volume, Pitch);
        }

        void SoundEmitter::Stop() {
            if(Channel) {
                FMOD_Channel_Stop(Channel);
                Channel = NULL;
            }
        }
    }
}
