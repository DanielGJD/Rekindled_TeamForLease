#include "ComponentSprite.h"
#include "Engine.h"

namespace ForLeaseEngine {
    namespace Components {
        Sprite::Sprite(Entity& parent) : Component(parent, ComponentType::Transform) {
            Visible = true;
            SpriteColor = Color(1, 1, 1);
            BlendingMode = BlendMode::ALPHA;
            //FlipX;
            //FlipY;
            AnimationActive = true;
            FrameRate = 24;
            AnimationSpeed = 1;
            //StartFrame;
            FrameTime = 0;
            CurrentFrame = 0;
        }
        Sprite::~Sprite() {}

        void Sprite::Update() {
            if(SpriteSource.size() > 0 && AnimationActive) {
                double dt = ForLease->FrameRateController().GetDt();
                FrameTime += dt * FrameRate * AnimationSpeed;
                if(static_cast<unsigned int>(FrameTime) >= SpriteSource.size()) {
                    FrameTime -= SpriteSource.size();
                }
                CurrentFrame = static_cast<unsigned int>(FrameTime);
            }
        }

        int Sprite::GetCurrentFrame() { return CurrentFrame; }

        void Sprite::SetCurrentFrame(unsigned int frame) {
            if(frame < SpriteSource.size()) {
                CurrentFrame = frame;
                FrameTime = frame;
            }
        }

        TextureRegion* Sprite::GetCurrentRegion() { return &SpriteSource[CurrentFrame]; }
    }
}
