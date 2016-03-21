/*!
    \file   ComponentLight.h
    \author Christopher Hudson
    \brief
        Defines the Light component.

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/


#ifndef COMPONENT_LIGHT_H
#define COMPONENT_LIGHT_H

#include "Component.h"
#include "Vector.h"
#include "Color.h"
#include "Mesh.h"
#include "Ray.h"
#include <unordered_set>

#ifndef PI
#define PI 3.14159265358979323846
#endif

namespace ForLeaseEngine {
    namespace Components {

        /*!
            \class Light

            \brief
                A class that ray casts from a point to create a light mesh
        */
        class Light : public Component {
            public:
                static const ComponentType Type = ComponentType::Light;
                virtual ComponentType GetType() { return Type; }

                bool Active;
                bool Visible;
                bool DrawOutline;
                Vector Offset;
                Vector Direction;
                float Angle;
                Color LightColor;
                BlendMode LightMode;
                float Radius;
                std::string LightTexture;

                Light(Entity& owner, bool active = true, bool visible = true, bool drawOutline = false,
                      Vector offset = Vector(), Vector direction = Vector(0, -1), float angle = PI / 2 ,
                      Color lightColor = Color(1, 1, 1, 1), BlendMode lightMode = BlendMode::ADDITIVE, float radius = 5, std::string lightTexture = "");
                ~Light();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
                Mesh* GetLightMesh();
                std::unordered_set<unsigned long> ComputeLighting();
            private:
                Point CheckRayAgainstWindow(Ray& ray, Point& tl, Point& tr, Point& br, Point& bl, Point& position);
                Mesh LightMesh;
        };
    }
}

#endif
