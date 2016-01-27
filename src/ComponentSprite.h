/*!
    \file   ComponentSprite.h
    \author Christopher Hudson

    \brief
        Defines a component for displaying an animated sprite

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "Color.h"
#include "TextureRegion.h"
#include "Component.h"
#include "Entity.h"
#include "Serializable.h"
#include "Serialize.h"
#include <vector>
#include <string>

namespace ForLeaseEngine {
    namespace Components {
        class Sprite : public Component {
            public:
                static const ComponentType Type = ComponentType::Sprite;
                virtual ComponentType GetType() { return Type; }
                Sprite(Entity& parent, std::string textureName = "", bool visible = true, Color const& spriteColor = Color(1, 1, 1, 1), BlendMode blendingMode = BlendMode::ALPHA,
                       bool flipX = false, bool flipY = false, bool animationActive = false, float frameRate = 24, float animationSpeed = 1, int startFrame = 0);
                ~Sprite();

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void Update();
                int GetCurrentFrame();
                void SetCurrentFrame(unsigned int frame);
                TextureRegion* GetCurrentRegion();

                friend std::ostream& operator<<(std::ostream& os, Sprite& rhs);

                bool Visible;
                Color SpriteColor;
                BlendMode BlendingMode;
                TextureRegion SpriteSource;
                bool FlipX;
                bool FlipY;
                bool AnimationActive;
                float FrameRate;
                float AnimationSpeed;
                int StartFrame;

                //////////////////////// Everything past here is temporary and subject to change//////////////
                void SetSpriteSource(std::string const& textureName);
                std::string GetSourceName();

            private:
                float FrameTime;
                unsigned int CurrentFrame;
        };
    }
}

#endif // SPRITE_H
