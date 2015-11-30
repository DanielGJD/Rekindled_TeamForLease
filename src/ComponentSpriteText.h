/*!
    \file   ComponentSpriteText.h
    \author Christopher Hudson

    \brief
        Defines a component for rendering text

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef SPRITETEXT_H
#define SPRITETEXT_H

#include <string>
#include "Font.h"
#include "Component.h"
#include "Color.h"
#include "Serializable.h"
#include "Serialize.h"

namespace ForLeaseEngine {
    namespace Components {
        class SpriteText : public Component {
            public:
                static const ComponentType Type = ComponentType::SpriteText;
                virtual ComponentType GetType() { return Type; }
                SpriteText(Entity& owner, const std::string& font);
                SpriteText(Entity& owner, const std::string& font, const std::string& text, const Color& textColor);

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void Update();

                std::string GetFont();
                std::string Text;
                Color TextColor;
            private:
                std::string TextFont;
        };
    }
}

#endif // SPRITETEXT_H
