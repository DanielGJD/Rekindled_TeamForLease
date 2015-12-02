/*!
    \file   ComponentEnemyAI.h
    \author Christopher Hudson

    \brief
        A component for an AI that does not move, and detects the player causing a loss condition if
        it sees the player for too long

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_ENEMY_AI
#define COMPONENT_ENEMY_AI

#include "Component.h"
#include "Color.h"
#include "Event.h"
#include <string>

namespace ForLeaseEngine {
    namespace Components {
        class EnemyAI : public Component {
            public:
                static const ComponentType Type = ComponentType::EnemyAI;

                bool Active;
                Color NoDetectionColor;
                Color DetectionColor;
                Color HappyColor;
                float DetectionDelay;
                std::string HatedEntityName;
                std::string LikedEntityName;
                std::string HatedSeenSound;
                std::string LikedSeenSound;

                EnemyAI(Entity& parent,
                        bool active = true,
                        Color const& noDetectionColor = Color(0, 1, 0, 0.3),
                        Color const& detectionColor = Color(1, 0, 0, 0.3),
                        Color const& happyColor = Color(0, 0, 1, 0.3),
                        float detectionDelay = 3,
                        std::string const& hatedEntityName = "",
                        std::string const& likedEntityName = "",
                        std::string const& hatedSeenSound = "",
                        std::string const& likedSeenSound = "");
                ~EnemyAI();

                void Initialize();
                static EnemyAI* Create(Entity& parent);

                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void OnEntitiesSeen(Event const* e);
            private:
                bool Happy;
                bool Angry;
                float DetectionTimer;
        };
    }
}

#endif // COMPONENT_ENEMY_AI
