/*!
    \file   Serialize.cpp
    \author Sam Montanari
    \date   10/12/15

    \brief
        Implementation of serialization class

    \see Serialize.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/
#include "Serialize.h"
#include "Exception.h"
#include <iostream>
#include <fstream>

namespace ForLeaseEngine
{
    /*!
        \brief
            Reads a JSON file into a node.

        \param filename
            Name of the file being read from.

        \return
            If the file was read successfully
    */
    bool Serializer::ReadFile(const std::string& filename)
    {
        try
        {
            std::ifstream file(filename);
            file >> node;
            return true;
        }

        catch (std::exception& e)
        {
            throw Exception(e.what());
        }
    }

    /*!
        \brief
            Writes a node to a file

        \param filename
            Name of the file being written to.
    */
    void Serializer::WriteFile(const std::string& filename) const
    {
        std::ofstream file(filename);
        file << node << std::endl;
    }

    /*!
        \brief
            Writes an integer to the node

        \param path
            String key paired with the stored value

        \param i
            Integer being stored
    */
    void Serializer::WriteInt(const std::string& path, const int i)
    {
        node[path] = i;
    }

    /*!
        \brief
            Writes an unsigned integer to the node

        \param path
            String key paired with the stored value

        \param u
            Unsigned integer being stored
    */
    void Serializer::WriteUint(const std::string& path, const unsigned u)
    {
        node[path] = u;
    }

    /*!
        \brief
            Writes a float to the node

        \param path
            String key paired with the stored value

        \param f
            Float being stored
    */
    void Serializer::WriteFloat(const std::string& path, const float f)
    {
        node[path] = f;
    }

    /*!
        \brief
            Writes a string to the node

        \param path
            String key paired with the stored value

        \param s
            String being stored
    */
    void Serializer::WriteString(const std::string& path, const std::string& s)
    {
        node[path] = s;
    }

    /*!
        \brief
            Writes a bool to the node

        \param path
            String key paired with the stored value

        \param b
            Boolean being stored
    */
    void Serializer::WriteBool(const std::string& path, const bool b)
    {
        node[path] = b;
    }

    /*!
        \brief
            Writes a Hcoord object to the node

        \param path
            String key paired with the stored value

        \param v
            Hcoord being stored
    */
    void Serializer::WriteVec(const std::string& path, const Hcoord& v)
    {
        node[path]["X"] = v[0];
        node[path]["Y"] = v[1];
    }

    /*!
        \brief
            Writes an array of ints to the node

        \param path
            String needed to access the array

        \param a
            Array to be stored. If using std::vector, pass &vec[0]

        \param size
            Size of the array
    */
    void Serializer::WriteIntArray(const std::string& path, const int* a, const int size)
    {
        Json::Value arrayNode(Json::arrayValue);

        for (int i = 0; i < size; i++)
            arrayNode.append(a[i]);

        node[path] = arrayNode;
    }

    /*!
        \brief
            Writes an array of unsigned ints to the node

        \param path
            String needed to access the array

        \param a
            Array to be stored. If using std::vector, pass &vec[0]

        \param size
            Size of the array
    */
    void Serializer::WriteUintArray(const std::string& path, const unsigned* a, const int size)
    {
        Json::Value arrayNode(Json::arrayValue);

        for (int i = 0; i < size; i++)
            arrayNode.append(a[i]);

        node[path] = arrayNode;
    }

    /*!
        \brief
            Writes an array of floats to the node

        \param path
            String needed to access the array

        \param a
            Array to be stored. If using std::vector, pass &vec[0]

        \param size
            Size of the array
    */
    void Serializer::WriteFloatArray(const std::string& path, const float* a, const int size)
    {
        Json::Value arrayNode(Json::arrayValue);

        for (int i = 0; i < size; i++)
            arrayNode.append(a[i]);

        node[path] = arrayNode;
    }

    /*!
        \brief
            Writes an array of strings to the node

        \param path
            String needed to access the array

        \param a
            Array to be stored. If using std::vector, pass &vec[0]

        \param size
            Size of the array
    */
    void Serializer::WriteStringArray(const std::string& path, const std::string* a, const int size)
    {
        Json::Value arrayNode(Json::arrayValue);

        for (int i = 0; i < size; i++)
            arrayNode.append(a[i]);

        node[path] = arrayNode;
    }

    /*!
        \brief
            Writes an array of bools to the node

        \param path
            String needed to access the array

        \param a
            Array to be stored

        \param size
            Size of the array
    */
    void Serializer::WriteBoolArray(const std::string& path, const bool* a, const int size)
    {
        Json::Value arrayNode(Json::arrayValue);

        for (int i = 0; i < size; i++)
            arrayNode.append(a[i]);

        node[path] = arrayNode;
    }

    /*!
        \brief
            Reads an integer from the node

        \param path
            String needed to access the value

        \param i
            Integer being read into
    */
    void Serializer::ReadInt(const std::string& path, int& i) const
    {
        i = node[path].asInt();
    }

    /*!
        \brief
            Reads an unsigned integer from the node

        \param path
            String needed to access the value

        \param u
            Unsigned integer being read into
    */
    void Serializer::ReadUint(const std::string& path, unsigned& u) const
    {
        u = node[path].asUInt();
    }

    /*!
        \brief
            Reads a float from the node

        \param path
            String needed to access the value

        \param f
            Float being read into
    */
    void Serializer::ReadFloat(const std::string& path, float& f) const
    {
        f = node[path].asFloat();
    }

    /*!
        \brief
            Reads a string from the node

        \param path
            String needed to access the value

        \param s
            String being read into
    */
    void Serializer::ReadString(const std::string& path, std::string& s) const
    {
        s = node[path].asString();
    }

    /*!
        \brief
            Reads a bool from the node

        \param path
            String needed to access the value

        \param b
            Boolean being read into
    */
    void Serializer::ReadBool(const std::string& path, bool& b) const
    {
        b = node[path].asBool();
    }

    /*!
        \brief
            Reads a Hcoord object from the node

        \param path
            String needed to access the value

        \param v
            Hcoord being read into
    */
    void Serializer::ReadVec(const std::string& path, Hcoord& v) const
    {
        v[0] = node[path]["X"].asFloat();
        v[1] = node[path]["Y"].asFloat();
    }

    /*!
        \brief
            Reads an array of ints from the node

        \param path
            String needed to access the array

        \param a
            Array being read into. If using std::vector, pass &vec[0]

        \param size
            Size of the array
    */
    void Serializer::ReadIntArray(const std::string& path, int* a) const
    {
        for (unsigned i = 0; i < node[path].size(); i++)
            a[i] = node[path][i].asInt();
    }

    /*!
        \brief
            Reads an array of unsigned ints from the node

        \param path
            String needed to access the array

        \param a
            Array being read into. If using std::vector, pass &vec[0]

        \param size
            Size of the array
    */
    void Serializer::ReadUintArray(const std::string& path, unsigned* a) const
    {
        for (unsigned i = 0; i < node[path].size(); i++)
            a[i] = node[path][i].asUInt();
    }

    /*!
        \brief
            Reads an array of floats from the node

        \param path
            String needed to access the array

        \param a
            Array being read into. If using std::vector, pass &vec[0]
    */
    void Serializer::ReadFloatArray(const std::string& path, float* a) const
    {
        for (unsigned i = 0; i < node[path].size(); i++)
            a[i] = node[path][i].asFloat();
    }

    /*!
        \brief
            Reads an array of strings from the node

        \param path
            String needed to access the array

        \param a
            Array being read into. If using std::vector, pass &vec[0]
    */
    void Serializer::ReadStringArray(const std::string& path, std::vector<std::string>& a) const
    {
        for (unsigned i = 0; i < node[path].size(); i++)
            a.push_back(node[path][i].asString());
    }

    /*!
        \brief
            Reads an array of bools from the node

        \param path
            String needed to access the array

        \param a
            Array being read into
    */
    void Serializer::ReadBoolArray(const std::string& path, bool* a) const
    {
        for (unsigned i = 0; i < node[path].size(); i++)
            a[i] = node[path][i].asBool();
    }


    /*!
        \brief
            Assignment operator for Serializer class

        \param rhs
            Serializer object on the right hand side of the operation

        \return
            Reference to the object being assigned to
    */
    Serializer& Serializer::operator=(const Serializer& rhs)
    {
        node = rhs.node;
        return *this;
    }

    /*!
        \brief
            Gets a child node of the JSON string

        \param path
            String needed to access the child

        \return
            Serializer object containing the child node
    */
    Serializer Serializer::GetChild(const std::string& path) const
    {
        Serializer result;
        result.node = node[path];
        return result;
    }

    /*!
        \brief
            Gets all child names under the parent

        \return
            Vector of strings containing all the child names
    */
    std::vector<std::string> Serializer::GetMemberNames() const
    {
        return node.getMemberNames();
    }

    /*!
        \brief
            Writes the child node to the parent node

        \param child
            Child node to write to the parent

        \param path
            Path used to access the child
    */
    void Serializer::Append(Serializer& child, const std::string& path)
    {
        node[path] = child.node;
    }

    Json::Value& Serializer::GetNode()
    {
        return node;
    }

    ArraySerializer::ArraySerializer(Serializer root) : Serializer()
    {
        node = Json::Value(Json::arrayValue);
        node = root.GetNode();
    }

    void ArraySerializer::Append(Serializer& child)
    {
        node.append(child.GetNode());
    }

    Serializer ArraySerializer::operator[](unsigned i)
    {
        Serializer result;
        result.GetNode() = node[i];
        return result;
    }

    Serializer ArraySerializer::operator[](unsigned i) const
    {
        Serializer result;
        result.GetNode() = node[i];
        return result;
    }

    unsigned ArraySerializer::Size() const
    {
        return node.size();
    }
}
