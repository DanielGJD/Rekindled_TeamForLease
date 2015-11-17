/*!
    \file   ComponentModel.h
    \author Christopher Hudson
    \date   10/05/15

    \brief
        Defines a class that holds data pertaining to a model

    \see ComponentModel.cpp
*/
#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

#include "Mesh.h"
#include "Texture.h"
#include "Component.h"
#include "Entity.h"
#include "Serializable.h"

namespace ForLeaseEngine {
    namespace Components {
        /*!
            \class Model

            \brief
                Contains data on a model
        */
        class Model : public Component {
            public:
                static const ComponentType Type = ComponentType::Model;
                virtual ComponentType GetType() { return Type; }
                Model(Entity& parent, bool visible, const std::string& mesh,
                      const std::string& texture, Color color = Color(1, 1, 1, 1),
                      BlendMode blend = BlendMode::NONE, bool drawEdges = false,
                      bool drawVertices = false, bool animationActive = false, float frameRate = 24.0f,
                      unsigned int currentFrame = 0, std::string const& currentAnimation = "");
                ~Model();

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void Update();

                void SetAnimation(std::string animation);
                std::string const& GetAnimation();
                void SetFrame(unsigned int frame);
                unsigned int GetFrame();
                float GetFrameTime();

                //! If the model is visible
                bool Visible;
                //! Mesh to use for the model
                std::string ModelMesh;
                //! Texture to apply to the mesh
                std::string ModelTexture;
                //! Color to apply to the model
                Color ModelColor;
                //! Blending mode to use for the model
                BlendMode BlendingMode;
                //! If edges should be drawn
                bool DrawEdges;
                //! If Vertices should be drawn
                bool DrawVertices;
                //! If the animation is active
                bool AnimationActive;
                //! Frame rate for animation
                float FrameRate;
            private:
                //! Current frame of animation
                unsigned int CurrentFrame;
                //! Current frame time
                float FrameTime;
                //! Current animation
                std::string CurrentAnimation;
        };
    }
}

#endif // COMPONENTMODEL_H
