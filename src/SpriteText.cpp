#include "SpriteText.h"
#include "Font.h"
#include <string>

namespace ForLeaseEngine {
    SpriteText::SpriteText(Font* font) : TextFont(font) {}
    SpriteText::SpriteText(Font* font, std::string text) : TextFont(font), Text(text) {}

    Font* SpriteText::GetFont() {
        return TextFont;
    }

    std::string& SpriteText::GetText() {
        return Text;
    }

    void SpriteText::SetText(const std::string& text) {
        Text = text;
    }
}

