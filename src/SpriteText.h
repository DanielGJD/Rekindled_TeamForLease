#ifndef SPRITETEXT_H
#define SPRITETEXT_H

#include <string>
#include "Font.h"

namespace ForLeaseEngine {
    class SpriteText {
        public:
            SpriteText(Font* font);
            SpriteText(Font* font, std::string text);

            Font* GetFont();
            std::string& GetText();
            void SetText(const std::string& text);
        private:
            Font* TextFont;
            std::string Text;

    };
}

#endif // SPRITETEXT_H
