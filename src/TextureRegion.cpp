/*!
    \file   TextureRegion.cpp
    \author Christopher Hudson

    \brief
        Defines a class that contains information defining a portion of a texture

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "TextureRegion.h"
#include "Serializable.h"
#include "Serialize.h"
#include <sstream>

namespace ForLeaseEngine {
    TextureRegion::TextureRegion() {
        TextureID = 0;
        Width = 0;
        Height = 0;
    }

    TextureRegion::TextureRegion(Texture* texture, int left, int right, int top, int bottom) {
        Source = texture->GetName();
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

    void TextureRegion::Serialize(Serializer& root) {
        Serializer textureRegion = root.GetChild("TextureRegion");
        textureRegion.WriteString("Source", Source);
        textureRegion.WriteInt("Width", Width);
        textureRegion.WriteInt("Height", Height);
        for(int i = 0; i < 4; ++i) {
            std::stringstream ss;
            ss << "UV" << i;
            Serializer uv = textureRegion.GetChild(ss.str());
            uv.WriteFloat("X", UV[i][0]);
            uv.WriteFloat("Y", UV[i][1]);
            textureRegion.Append(uv, ss.str());
        }
        root.Append(textureRegion, "TextureRegion");
    }

    void TextureRegion::Deserialize(Serializer& root) {
        Serializer textureRegion = root.GetChild("TextureRegion");
        textureRegion.ReadString("Source", Source);
        textureRegion.ReadInt("Width", Width);
        textureRegion.ReadInt("Height", Height);
        for(int i = 0; i < 4; ++i) {
            std::stringstream ss;
            ss << "UV" << i;
            Serializer uv = textureRegion.GetChild(ss.str());
            uv.ReadFloat("X", UV[i][0]);
            uv.ReadFloat("Y", UV[i][1]);
        }
    }

    GLuint TextureRegion::GetTextureID() {
        return TextureID;
    }

    std::string& TextureRegion::GetTexture() {
        return Source;
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
