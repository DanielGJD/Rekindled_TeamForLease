#include "TextureRegion.h"

namespace ForLeaseEngine {
    TextureRegion::TextureRegion() {

    }

    TextureRegion::TextureRegion(Texture* texture, int left, int right, int top, int bottom) {
        //Source = texture;
        TextureID = texture->GetID();
        Width = right - left;
        Height = bottom - top;
        UV[0][0] = static_cast<float>(right) / texture->GetWidth();
        UV[1][0] = static_cast<float>(left) / texture->GetWidth();
        UV[2][0] = static_cast<float>(left) / texture->GetWidth();
        UV[3][0] = static_cast<float>(right) / texture->GetWidth();

        UV[0][1] = static_cast<float>(top) / texture->GetHeight();
        UV[1][1] = static_cast<float>(top) / texture->GetHeight();
        UV[2][1] = static_cast<float>(bottom) / texture->GetHeight();
        UV[3][1] = static_cast<float>(bottom) / texture->GetHeight();
    }

    GLuint TextureRegion::GetTextureID() {
        return TextureID;
    }

    int TextureRegion::GetWidth() {
        return Width;
    }

    int TextureRegion::GetHeight() {
        return Height;
    }

    Point* TextureRegion::GetUV() {
        return UV;
    }
}
