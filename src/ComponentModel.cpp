/*!
    \file   ComponentModel.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Implementation of a class that stores data on a model

    \see ComponentModel.h
*/

#include "ComponentModel.h"

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
        Model::Model(Entity& parent, Mesh* mesh, Texture* texture, Color color, BlendMode blend)
                    : Component(parent, ComponentType::Model, ComponentType::Transform),
                      ModelMesh(mesh), ModelTexture(texture), ModelColor(color), BlendingMode(blend) {}

        /*!
            \brief
                Destructor, no functionality
        */
        Model::~Model() {}

        /*!
            \brief
                Updates model, no functionality
        */
        void Model::Update() {}
    }
}
