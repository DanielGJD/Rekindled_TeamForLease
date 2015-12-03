#ifndef COMPONENT_FADE_WITH_DISTANCE
#define COMPONENT_FADE_WITH_DISTANCE

#include "Component.h"

namespace ForLeaseEngine {
    namespace Components {
        class FadeWithDistance : public Component {
            public:
                static const ComponentType Type = ComponentType::FadeWithDistance;
                bool Active;
                bool XDirection;
                bool YDirection;
                float FadeBeginDistance;
                float FadeEndDistance;
                unsigned long TrackedEntityID;

                FadeWithDistance(Entity& owner, bool active = true, bool xdirection = true, bool ydirection = true,
                                 float fadeBeginDistance = 0, float fadeEndDistace = 0, unsigned long trackedEntityID = 0);
                ~FadeWithDistance();

                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
            private:
        };
    }
}

#endif // COMPONENT_FADE_WITH_DISTANCE
