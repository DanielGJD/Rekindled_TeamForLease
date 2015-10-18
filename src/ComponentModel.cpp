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
        Model::Model(Entity& parent, bool visible, const std::string& mesh, const std::string& texture, Color color, BlendMode blend)
                    : Component(parent, ComponentType::Model, ComponentType::Transform),
                      Visible(visible), ModelMesh(mesh), ModelTexture(texture), ModelColor(color), BlendingMode(blend) {}

        /*!
            \brief
                Destructor, no functionality
        */
        Model::~Model() {}

        void Model::Serialize(Serializer& root) {
            Serializer model = root.GetChild("Model");
            model.WriteBool("Visible", Visible);
            model.WriteString("Mesh", ModelMesh);
            model.WriteString("Texture", ModelTexture);
            ModelColor.Serialize(model);
            model.WriteInt("BlendingMode", BlendingMode);
            root.Append(model, "Model");
        }

        void Model::Deserialize(Serializer& root) {
            Serializer model = root.GetChild("Model");
            model.ReadBool("Visible", Visible);
            model.ReadString("Mesh", ModelMesh);
            model.ReadString("Texture", ModelTexture);
            ModelColor.Deserialize(model);
            int blend;
            model.ReadInt("BlendingMode", blend);
            BlendingMode = static_cast<BlendMode>(blend);
        }

        /*!
            \brief
                Updates model, no functionality
        */
        void Model::Update() {}
    }
}
