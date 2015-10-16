#include "Serialize.h"
#include <fstream>

namespace ForLeaseEngine
{
    void Serializer::ReadFile(const std::string& filename)
    {
        std::ifstream file(filename);
        file >> node;
    }

    void Serializer::WriteFile(const std::string& filename)
    {
        std::ofstream file(filename);
        file << node << std::endl;
    }

    void Serializer::WriteInt(const std::string& path, int i)
    {
        node[path] = i;
    }

    void Serializer::WriteUint(const std::string& path, unsigned u)
    {
        node[path] = u;
    }

    void Serializer::WriteFloat(const std::string& path, float f)
    {
        node[path] = f;
    }

    void Serializer::WriteString(const std::string& path, const std::string& s)
    {
        node[path] = s;
    }
    void Serializer::WriteBool(const std::string& path, bool b)
    {
        node[path] = b;
    }

    void Serializer::WriteVec(const std::string& path, Hcoord& v)
    {
        node[path]["X"] = v[0];
        node[path]["Y"] = v[1];
    }

    void Serializer::ReadInt(const std::string& path, int& i)
    {
        i = node[path].asInt();
    }

    void Serializer::ReadUint(const std::string& path, unsigned& u)
    {
        u = node[path].asUInt();
    }

    void Serializer::ReadFloat(const std::string& path, float& f)
    {
        f = node[path].asFloat();
    }

    void Serializer::ReadString(const std::string& path, std::string& s)
    {
        s = node[path].asString();
    }

    void Serializer::ReadBool(const std::string& path, bool& b)
    {
        b = node[path].asBool();
    }

    Serializer& Serializer::operator=(const Serializer& rhs)
    {
        node = rhs.node;
        return *this;
    }

    Serializer Serializer::GetChild(const std::string& path)
    {
        Serializer result;
        result.node = node[path];
        return result;
    }

    std::vector<std::string> Serializer::GetMemberNames()
    {
        return node.getMemberNames();
    }

    void Serializer::Append(Serializer& child, const std::string& path)
    {
        node[path] = child.node;
    }
}
