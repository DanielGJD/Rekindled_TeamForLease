/*!
    \file   ComponentVisionCone.h
    \author Christopher Hudson

    \brief
        Defines a component representing a vision cone

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_VISION_CONE_H
#define COMPONENT_VISION_CONE_H

#include "Component.h"
#include "Serializable.h"
#include "Serialize.h"
#include "Vector.h"
#include "Color.h"
#include "Mesh.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

namespace ForLeaseEngine {
    namespace Components {
        class VisionCone : public Component {
            public:
                static const ComponentType Type = ComponentType::VisionCone;

                bool Active;
                bool Visible;
                bool DrawOutline;
                Vector Offset;
                float Radius;
                Vector Direction;
                float Angle;
                Color IndicatorColor;

                VisionCone(Entity& parent,
                           bool active = true,
                           bool visible = true,
                           bool drawOutline = false,
                           Vector const& offset = Vector(0, 0),
                           float radius = 4,
                           Vector const& direction = Vector(1, 0),
                           float angle = PI / 2,
                           Color const& indicatorColor = Color(0.5f, 0, 0.5f, 0.2f));
                ~VisionCone();
                virtual ComponentType GetType();
                void Update();

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                Mesh* GetVisionMesh();
            private:
                Mesh ViewMesh;
        };
    }
}

#endif // COMPONENT_VISION_CONE_H


