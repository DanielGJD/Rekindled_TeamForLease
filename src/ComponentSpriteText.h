#ifndef SPRITETEXT_H
#define SPRITETEXT_H

#include <string>
#include "Font.h"
#include "Component.h"
#include "Color.h"

namespace ForLeaseEngine {
    namespace Components {
        class SpriteText : public Component {
            public:
                SpriteText(Entity& owner, Font* font);
                SpriteText(Entity& owner, Font* font, std::string text, const Color& textColor);
                void Update();

                Font* GetFont();
                std::string Text;
                Color TextColor;
            private:
                Font* TextFont;
        };
    }
}

#endif // SPRITETEXT_H
