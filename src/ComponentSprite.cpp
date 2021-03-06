/*!
    \file   ComponentSprite.cpp
    \author Christopher Hudson

    \brief
        Defines a component for an animated sprite

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentSprite.h"
#include "Engine.h"
#include "TextureRegion.h"
#include <sstream>

namespace ForLeaseEngine {
    namespace Components {
        Sprite::Sprite(Entity& parent, std::string textureName, bool visible, Color const& spriteColor, BlendMode blendingMode,
                       bool flipX, bool flipY, bool animationActive, float frameRate, float animationSpeed, int startFrame) :
                       Component(parent, ComponentType::Transform),
                       Visible(visible), SpriteColor(spriteColor), BlendingMode(blendingMode),
                       SpriteSource(TextureRegion()), FlipX(flipX), FlipY(flipY),
                       AnimationActive(animationActive), FrameRate(FrameRate), AnimationSpeed(AnimationSpeed), StartFrame(startFrame) {
            SetSpriteSource(textureName);
        }

        Sprite::~Sprite() {}

        void Sprite::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer sprite = root.GetChild("Sprite");
            sprite.WriteBool("Visible", Visible);
            SpriteColor.Serialize(sprite);
            sprite.WriteInt("BlendingMode", BlendingMode);
            sprite.WriteInt("NumFrames", 1/* Don't even worry about this, needed for animations later*/);
            /*for(unsigned int i = 0; i < SpriteSource.size(); ++i) {
                std::stringstream ss;
                ss << "TextureRegion" << i;
                Serializer textureRegion = root.GetChild(ss.str());
                SpriteSource[i].Serialize(textureRegion);
                root.Append(textureRegion, ss.str());
            }*/
            Serializer textureRegion = sprite.GetChild("TextureRegion");
            SpriteSource.Serialize(textureRegion);
            sprite.Append(textureRegion, "TextureRegion");
            sprite.WriteBool("FlipX", FlipX);
            sprite.WriteBool("FlipY", FlipY);
            sprite.WriteBool("AnimationActive", AnimationActive);
            sprite.WriteFloat("FrameRate", FrameRate);
            sprite.WriteFloat("AnimationSpeed", AnimationSpeed);
            sprite.WriteInt("StartFrame", StartFrame);
            sprite.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(sprite, "Sprite");
        }

        void Sprite::Deserialize(Serializer& root) {
            Serializer sprite = root.GetChild("Sprite");
            sprite.ReadBool("Visible", Visible);
            SpriteColor.Deserialize(sprite);
            int blendMode;
            sprite.ReadInt("BlendingMode", blendMode);
            BlendingMode = static_cast<BlendMode>(blendMode);
            int numFrames;
            sprite.ReadInt("NumFrames", numFrames);
            /*SpriteSource.clear();
            for(int i = 0; i < numFrames; ++i) {
                std::stringstream ss;
                ss << "TextureRegion" << i;
                Serializer textureRegion = root.GetChild(ss.str());
                TextureRegion region;
                region.Deserialize(textureRegion);
                SpriteSource.push_back(region);
            }*/
            Serializer textureRegion = sprite.GetChild("TextureRegion");
            SpriteSource.Deserialize(textureRegion);
            sprite.ReadBool("FlipX", FlipX);
            sprite.ReadBool("FlipY", FlipY);
            sprite.ReadBool("AnimationActive", AnimationActive);
            sprite.ReadFloat("FrameRate", FrameRate);
            sprite.ReadFloat("AnimationSpeed", AnimationSpeed);
            sprite.ReadInt("StartFrame", StartFrame);
        }

        void Sprite::Update() {
            /*if(SpriteSource.size() > 0 && AnimationActive) {
                double dt = ForLease->FrameRateController().GetDt();
                FrameTime += dt * FrameRate * AnimationSpeed;
                if(static_cast<unsigned int>(FrameTime) >= SpriteSource.size()) {
                    FrameTime -= SpriteSource.size();
                }
                CurrentFrame = static_cast<unsigned int>(FrameTime);
            }*/
        }

        int Sprite::GetCurrentFrame() { return CurrentFrame; }

        void Sprite::SetCurrentFrame(unsigned int frame) {
            /*if(frame < SpriteSource.size()) {
                CurrentFrame = frame;
                FrameTime = frame;
            }*/
        }

        TextureRegion* Sprite::GetCurrentRegion() { return &SpriteSource; }

        void Sprite::SetSpriteSource(std::string const& textureName) {
            Texture* texture = ForLease->Resources.GetTexture(textureName);
            if(texture)
                SpriteSource = TextureRegion(textureName, 0, texture->GetWidth(), 0, texture->GetHeight());
        }

        std::string Sprite::GetSourceName() {
            return SpriteSource.GetTexture();
        }

        std::ostream& operator<<(std::ostream& os, Sprite& rhs) {
            os << "Visible: " << rhs.Visible << std::endl
               << "Color: No output yet" << std::endl
               << "Blend Mode: " << rhs.BlendingMode << std::endl
               << "Frames: " << 1 /*Don't forget this hack here*/ << std::endl
               << "FlipX: " << rhs.FlipX << std::endl
               << "FlipY: " << rhs.FlipY << std::endl
               << "Animation Active: " << rhs.AnimationActive << std::endl
               << "Frame Rate: " << rhs.FrameRate << std::endl
               << "Animation Speed: " << rhs.AnimationSpeed << std::endl
               << "Start Frame: " << rhs.StartFrame << std::endl
               << "Current Frame: " << rhs.CurrentFrame << std::endl
               << "Frame Time: " << rhs.FrameTime;
            return os;
        }
    }
}
