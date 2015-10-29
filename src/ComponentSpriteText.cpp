#include "ComponentSpriteText.h"
#include "Font.h"
#include <string>
#include "Component.h"

namespace ForLeaseEngine {
    namespace Components {
        SpriteText::SpriteText(Entity& owner, const std::string& font) : Component(owner, ComponentType::Transform), TextFont(font) {}
        SpriteText::SpriteText(Entity& owner, const std::string& font, const std::string& text, const Color& textColor)
                              : Component(owner, ComponentType::Transform), Text(text), TextColor(textColor), TextFont(font) {}

        void SpriteText::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer spriteText = root.GetChild("SpriteText");
            spriteText.WriteString("Font", TextFont);
            spriteText.WriteString("Text", Text);
            TextColor.Serialize(spriteText);
            spriteText.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(spriteText, "SpriteText");
        }

        void SpriteText::Deserialize(Serializer& root) {
            Serializer spriteText = root.GetChild("SpriteText");
            spriteText.ReadString("Font", TextFont);
            spriteText.ReadString("Text", Text);
            TextColor.Deserialize(spriteText);
        }

        void SpriteText::Update() {}


        std::string SpriteText::GetFont() {
            return TextFont;
        }
    }
}

