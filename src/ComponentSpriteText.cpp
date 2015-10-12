#include "ComponentSpriteText.h"
#include "Font.h"
#include <string>
#include "Component.h"

namespace ForLeaseEngine {
    namespace Components {
        SpriteText::SpriteText(Entity& owner, Font* font) : Component(owner, ComponentType::Transform), TextFont(font) {}
        SpriteText::SpriteText(Entity& owner, Font* font, std::string text, const Color& textColor)
                              : Component(owner, ComponentType::Transform), Text(text), TextColor(textColor), TextFont(font) {}

        void SpriteText::Update() {}


        Font* SpriteText::GetFont() {
            return TextFont;
        }
    }
}

