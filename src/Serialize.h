#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "json.h"
#include "Vector.h"
#include <vector>

namespace ForLeaseEngine
{
    class Serializer
    {
    private:
        Json::Value node;
    public:
        void ReadFile(const std::string& filename);
        void WriteFile(const std::string& filename);
        void WriteInt(const std::string& path, int i);
        void WriteUint(const std::string& path, unsigned u);
        void WriteFloat(const std::string& path, float f);
        void WriteString(const std::string& path, const std::string& s);
        void WriteBool(const std::string& path, bool b);
        void WriteVec(const std::string& path, Hcoord& v);
        void ReadInt(const std::string& path, int& i);
        void ReadUint(const std::string& path, unsigned& u);
        void ReadFloat(const std::string& path, float& f);
        void ReadString(const std::string& path, std::string& s);
        void ReadBool(const std::string& path, bool& b);
        Serializer& operator=(const Serializer& rhs);
        Serializer GetChild(const std::string& path);
        void Append(Serializer& child, const std::string& path);
        std::vector<std::string> GetMemberNames();
    };
}

#endif // SERIALIZER_H
