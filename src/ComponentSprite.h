#ifndef SPRITE_H
#define SPRITE_H

#include "Color.h"
#include "TextureRegion.h"
#include "Component.h"
#include "Entity.h"
#include <vector>

namespace ForLeaseEngine {
    namespace Components {
        class Sprite : public Component {
            public:
                static const ComponentType Type = ComponentType::Sprite;
                Sprite(Entity& parent);
                ~Sprite();

                void Update();
                int GetCurrentFrame();
                void SetCurrentFrame(unsigned int frame);
                TextureRegion* GetCurrentRegion();

                bool Visible;
                Color SpriteColor;
                BlendMode BlendingMode;
                std::vector<TextureRegion> SpriteSource;
                //bool FlipX;
                //bool FlipY;
                bool AnimationActive;
                float FrameRate;
                float AnimationSpeed;
                //int StartFrame;

            private:
                float FrameTime;
                unsigned int CurrentFrame;
        };
    }
}

#endif // SPRITE_H
