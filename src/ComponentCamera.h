#ifndef COMPONENTCAMERA_H
#define COMPONENTCAMERA_H

#include "Component.h"
#include "Matrix.h"

namespace ForLeaseEngine {
    namespace Components {
        class Camera : public Component {
            public:
                float Near;
                float Far;
                float Size;

                Camera(Entity& parent, float near, float far, float size);
                ~Camera();
                void Update();
            private:
        };
    }
}

#endif // COMPONENTCAMERA_H
