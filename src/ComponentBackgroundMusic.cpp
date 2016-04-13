/*!
    \file   ComponentBackgroundMusic.cpp
    \author Christopher Hudson

    \brief
        Defines a component to control the background music

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentBackgroundMusic.h"
#include "SoundEmitter.h"

namespace ForLeaseEngine {
    namespace Components {
        BackgroundMusic::BackgroundMusic(Entity& parent, std::string const& musicName) : Component(parent), MusicName(musicName), Active(true)
        {
            std::cout << musicName << std::endl;
        }

        BackgroundMusic::~BackgroundMusic() {
            ForLease->sound->StopSound(MusicName);
        }

        ComponentType BackgroundMusic::GetType() { return Type; }

        void BackgroundMusic::Update() {
            if(Active) {
                ForLease->sound->PlayEvent(MusicName);
                ForLease->sound->Volume(1.0f, MusicName);
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
