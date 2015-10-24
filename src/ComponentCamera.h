#ifndef COMPONENTCAMERA_H
#define COMPONENTCAMERA_H

#include "Component.h"
#include "Matrix.h"
#include "Serializable.h"
#include "Serialize.h"

namespace ForLeaseEngine {
    namespace Components {
        class Camera : public Component {
            public:
                static const ComponentType Type = ComponentType::Camera;
                virtual ComponentType GetType() { return Type; }
                float Near;
                float Far;
                float Size;

                Camera(Entity& parent, float near, float far, float size);
                ~Camera();
                void Update();

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                Point WorldToScreen(const Point& point);
                Point ScreenToWorld(const Point& point);
            private:
        };
    }
}

#endif // COMPONENTCAMERA_H
