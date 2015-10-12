#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "../libs/jsoncpp/json.h"
#include "Vector.h"
#include <vector>

namespace ForLeaseEngine
{
    class Serializer
    {
    private:
        Json::Value node;
    public:
        void ReadFile(std::string& filename);
        void WriteFile(std::string& filename);
        void WriteInt(std::string& path, int i);
        void WriteUint(std::string& path, unsigned u);
        void WriteFloat(std::string& path, float f);
        void WriteString(std::string& path, std::string& s);
        void WriteBool(std::string& path, bool b);
        void WriteVec(std::string& path, Hcoord& v);
        void ReadInt(std::string& path, int& i);
        void ReadUint(std::string& path, unsigned& u);
        void ReadFloat(std::string& path, float& f);
        void ReadString(std::string& path, std::string& s);
        void ReadBool(std::string& path, bool& b);
        Serializer& operator=(const Serializer& rhs);
        Serializer GetChild(std::string& path);
        void Append(Serializer& child, std::string& path);
        std::vector<std::string> GetMemberNames();
    };
}

#endif // SERIALIZER_H
