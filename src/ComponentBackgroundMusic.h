/*!
    \file   ComponentBackgroundMusic.h
    \author Christopher Hudson

    \brief
        Defines a component to control the background music

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_BACKGROUND_MUSIC_H
#define COMPONENT_BACKGROUND_MUSIC_H

#include "Component.h"
#include "ComponentSoundEmitter.h"
#include "Entity.h"
#include <string>

namespace ForLeaseEngine {
    namespace Components {
        class BackgroundMusic : public Component {
            public:
                static const ComponentType Type = ComponentType::BackgroundMusic;
                std::string MusicName;

                BackgroundMusic(Entity& parent, std::string const& musicName = "");
                ~BackgroundMusic();
                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
            private:
                bool Active;
        };
    }
}

#endif // COMPONENT_BACKGROUND_MUSIC_H
