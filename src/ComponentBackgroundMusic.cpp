/*!
    \file   ComponentBackgroundMusic.cpp
    \author Christopher Hudson

    \brief
        Defines a component to control the background music

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentBackgroundMusic.h"
#include "ComponentSoundEmitter.h"

namespace ForLeaseEngine {
    namespace Components {
        BackgroundMusic::BackgroundMusic(Entity& parent, std::string const& musicName)
                                        : Component(parent, ComponentType::SoundEmitter), MusicName(musicName), Active(true) {}

        BackgroundMusic::~BackgroundMusic() {}

        ComponentType BackgroundMusic::GetType() { return Type; }

        void BackgroundMusic::Update() {
            if(Active) {
                Parent.GetComponent<Components::SoundEmitter>()->Play(MusicName);
                Active = false;
            }
        }

        void BackgroundMusic::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer bgMusic = root.GetChild("BackgroundMusic");
            bgMusic.WriteUint("Type", static_cast<unsigned int>(Type));
            bgMusic.WriteString("MusicName", MusicName);
            root.Append(bgMusic, "BackgroundMusic");
        }

        void BackgroundMusic::Deserialize(Serializer& root) {
            Serializer bgMusic = root.GetChild("BackgroundMusic");
            bgMusic.ReadString("MusicName", MusicName);
        }
    }
}
