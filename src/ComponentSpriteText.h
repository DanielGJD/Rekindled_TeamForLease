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
        class SpriteText : public Component, public Serializable {
            public:
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
