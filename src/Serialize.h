/*!
    \file   Serialize.h
    \author Sam Montanari
    \date   10/12/15

    \brief
        Defines serialization class

    \see Serialize.cpp
*/

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "json.h"
#include "Vector.h"
#include <vector>

namespace ForLeaseEngine
{
    /*!
        \class Serializer

        \brief
            Writes JSON files for serialization
    */
    class Serializer
    {
    private:
        //! Container with the formatted JSON string
        Json::Value node;
    public:
        void ReadFile(const std::string& filename);
        void WriteFile(const std::string& filename) const;
        void WriteInt(const std::string& path, const int i);
        void WriteUint(const std::string& path, const unsigned u);
        void WriteFloat(const std::string& path, const float f);
        void WriteString(const std::string& path, const std::string& s);
        void WriteBool(const std::string& path, const bool b);
        void WriteVec(const std::string& path, const Hcoord& v);
        void ReadInt(const std::string& path, int& i) const;
        void ReadUint(const std::string& path, unsigned& u) const;
        void ReadFloat(const std::string& path, float& f) const;
        void ReadString(const std::string& path, std::string& s) const;
        void ReadBool(const std::string& path, bool& b) const;
        void ReadVec(const std::string& path, Hcoord& v) const;
        Serializer& operator=(const Serializer& rhs);
        Serializer GetChild(const std::string& path) const;
        void Append(Serializer& child, const std::string& path);
        std::vector<std::string> GetMemberNames() const;
    };
}

#endif // SERIALIZER_H
