/*!
    \file   ComponentModel.cpp
    \author Christopher Hudson
    \date   11/21/15

    \brief
        Implementation of a class that stores data on a model

    \see ComponentModel.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentModel.h"
#include "Engine.h"
#include "MeshAnimation.h"

namespace ForLeaseEngine {
    namespace Components {
        /*!
            \brief
                Creates a model

            \param parent
                Parent entity

            \param mesh
                Mesh resource

            \param texture
                Texture resource

            \param color
                Model color

            \param blend
                Blend mode for model
        */
        Model::Model(Entity& parent, bool visible, bool flipX, bool flipY, const std::string& mesh,
                     const std::string& texture, Color color, BlendMode blend,
                     bool drawEdges, bool drawVertices, bool animationActive, bool looping, float frameRate,
                     unsigned int currentFrame, std::string const& currentAnimation)
                    : Component(parent, ComponentType::Transform),
                      Visible(visible), FlipY(flipY), FlipX(flipX), ModelMesh(mesh), ModelTexture(texture), ModelColor(color),
                      BlendingMode(blend), DrawEdges(drawEdges), DrawVertices(drawVertices),
                      AnimationActive(animationActive), Looping(looping), FrameRate(frameRate), CurrentFrame(currentFrame),
                      FrameTime(static_cast<float>(currentFrame) / frameRate), CurrentAnimation(currentAnimation){}

        /*!
            \brief
                Destructor, no functionality
        */
        Model::~Model() {}

        void Model::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer model = root.GetChild("Model");
            model.WriteBool("Visible", Visible);
            model.WriteBool("FlipX", FlipX);
            model.WriteBool("FlipY", FlipY);
            model.WriteString("Mesh", ModelMesh);
            model.WriteString("Texture", ModelTexture);
            ModelColor.Serialize(model);
            model.WriteInt("BlendingMode", BlendingMode);
            model.WriteBool("DrawEdges", DrawEdges);
            model.WriteBool("DrawVertices", DrawVertices);
            model.WriteBool("AnimationActive", AnimationActive);
            model.WriteBool("Looping", Looping);
            model.WriteFloat("FrameRate", FrameRate);
            model.WriteUint("CurrentFrame", CurrentFrame);
            model.WriteUint("Type", static_cast<unsigned>(Type));
            model.WriteString("CurrentAnimation", CurrentAnimation);
            root.Append(model, "Model");
        }

        void Model::Deserialize(Serializer& root) {
            Serializer model = root.GetChild("Model");
            model.ReadBool("Visible", Visible);
            model.ReadBool("FlipX", FlipX);
            model.ReadBool("FlipY", FlipY);
            model.ReadString("Mesh", ModelMesh);
            model.ReadString("Texture", ModelTexture);
            ModelColor.Deserialize(model);
            int blend;
            model.ReadInt("BlendingMode", blend);
            BlendingMode = static_cast<BlendMode>(blend);
            model.ReadBool("DrawEdges", DrawEdges);
            model.ReadBool("DrawVertices", DrawVertices);
            model.ReadBool("AnimationActive", AnimationActive);
            model.ReadBool("Looping", Looping);
            model.ReadFloat("FrameRate", FrameRate);
            model.ReadUint("CurrentFrame", CurrentFrame);
            model.ReadString("CurrentAnimation", CurrentAnimation);
        }

        /*!
            \brief
                Updates model frame time
        */
        void Model::Update() {
            if(AnimationActive && CurrentAnimation.compare("") != 0) {
                FrameTime += ForLease->FrameRateController().GetDt();
                unsigned int frameCount = ForLease->Resources.GetMeshAnimation(CurrentAnimation)->GetFrameCount();
                if(Looping) {
                    while(FrameTime >= frameCount / FrameRate) {
                        FrameTime -= frameCount / FrameRate;
                    }
                }
                else {
                    if(FrameTime >= (frameCount - 1) / FrameRate) {
                        SetFrame(frameCount);
                        AnimationActive = false;
                    }
                }
                CurrentFrame = static_cast<int>(FrameTime * FrameRate);
            }
        }

        /*!
            \brief
                Sets the current animation

            \param animation
                Name of the animation to use
        */
        void Model::SetAnimation(std::string animation) {
            FrameTime = 0;
            CurrentFrame = 0;
            CurrentAnimation = animation;
        }

        /*!
            \brief
                Gets the name of the current animation

            \return
                Name of the current animation
        */
        std::string const& Model::GetAnimation() {
            return CurrentAnimation;
        }

        /*!
            \brief
                Sets the current frame

            \param frame
                Frame to go to
        */
        void Model::SetFrame(unsigned int frame) {
            if(CurrentAnimation.compare("") != 0) {
                unsigned int frameCount = ForLease->Resources.GetMeshAnimation(CurrentAnimation)->GetFrameCount();
                if(frame >= frameCount) {
                    CurrentFrame = frameCount - 1;
                }
                else {
                    CurrentFrame = frame;
                }
                FrameTime = CurrentFrame / FrameRate;
            }
        }

        /*!
            \brief
                Gets the current frame

            \return
                Current frame
        */
        unsigned int Model::GetFrame() {
            return CurrentFrame;
        }

        /*!
            \brief
                Gets the current frame time

            \return
                Current frame time
        */
        float Model::GetFrameTime() {
            return FrameTime;
        }
    }
}
