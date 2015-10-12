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
                Model(Entity& parent, Mesh* mesh, Texture* texture, Color color = Color(1, 1, 1, 1), BlendMode blend = BlendMode::NONE);
                ~Model();

                void Update();

                //! If the model is visible
                bool Visible;
                //! Mesh to use for the model
                Mesh* ModelMesh;
                //! Texture to apply to the mesh
                Texture* ModelTexture;
                //! Color to apply to the model
                Color ModelColor;
                //! Blending mode to use for the model
                BlendMode BlendingMode;
            private:
        };
    }
}

#endif // COMPONENTMODEL_H
