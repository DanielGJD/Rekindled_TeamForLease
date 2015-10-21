/*!
    \file   Mesh.cpp
    \author Christopher Hudson
    \date   10/03/15

    \brief
        Defines classes to hold data about a triangular face

    \see Mesh.h
*/

#include "Exception.h"
#include "Edge.h"
#include "Face.h"
#include "Color.h"
#include "Mesh.h"
#include <sstream>
#include <iostream>

namespace ForLeaseEngine {
    /*!
        \brief
            Creates a mesh that has the given number of vertices, edges and faces

        \param numVerts
            Number of vertices

        \param numEdges
            Number of edges

        \param numFaces
            Number of faces
    */
    Mesh::Mesh(int numVerts, int numEdges, int numFaces) : VertexCount(numVerts), EdgeCount(numEdges), FaceCount(numFaces) {
        /*Vertices = new Point[numVerts];
        UVs = new Point[numVerts];
        Edges = new IndexedEdge[numEdges];
        Faces = new IndexedFace[numFaces];
        FaceColors = new Color[numFaces];*/
        Vertices.resize(numVerts);
        UVs.resize(numVerts);
        Edges.resize(numEdges);
        Faces.resize(numFaces);
        FaceColors.resize(numFaces);

    }

    Mesh::Mesh() : VertexCount(0), EdgeCount(0), FaceCount(0) {}

    /*!
        \brief
            Cleans up resources created by the mesh
    */
    Mesh::~Mesh() {
        /*if(Vertices)
            delete Vertices;
        if(UVs)
            delete UVs;
        if(Edges)
            delete Edges;
        if(Faces)
            delete Faces;
        if(FaceColors)
            delete FaceColors;*/
    }

    void Mesh::AddVertex(float x, float y, float u, float v) {
        AddVertex(Point(x, y), Point(u, v));
    }

    void Mesh::AddVertex(const Point& vertex, const Point& uv) {
        Vertices.push_back(vertex);
        UVs.push_back(uv);
        ++VertexCount;
    }

    void Mesh::AddEdge(int v1, int v2) {
        AddEdge(IndexedEdge(v1, v2));
    }

    void Mesh::AddEdge(const IndexedEdge& edge) {
        Edges.push_back(edge);
        ++EdgeCount;
    }

    void Mesh::AddFace(int v1, int v2, int v3, float r, float g, float b, float a) {
        AddFace(IndexedFace(v1, v2, v3), Color(r, g, b, a));
    }

    void Mesh::AddFace(const IndexedFace& face, const Color& color) {
        Faces.push_back(face);
        FaceColors.push_back(color);
        ++FaceCount;
    }

    void Mesh::DeleteVertex(int index) {
        if(index < Vertices.size()) {
            std::vector<IndexedEdge>::iterator ei = Edges.begin();
            while(ei != Edges.end()) {
                IndexedEdge& edge = *ei;
                if(edge.Indices[0] == index ||
                   edge.Indices[1] == index) {
                    ei = Edges.erase(ei);
                }
                else {
                    if(edge.Indices[0] > index)
                        --edge.Indices[0];
                    if(edge.Indices[1] > index)
                        --edge.Indices[1];
                    ++ei;
                }
            }

            int i = 0;
            auto fi = Faces.begin();
            while(fi != Faces.end()) {
                IndexedFace& face = *fi;
                if(face.Indices[0] == index ||
                   face.Indices[1] == index ||
                   face.Indices[2] == index) {
                    fi = Faces.erase(fi);
                    FaceColors.erase(FaceColors.begin() + i);
                }
                else {
                    if(face.Indices[0] > index)
                        --face.Indices[0];
                    if(face.Indices[1] > index)
                        --face.Indices[1];
                    if(face.Indices[2] > index)
                        --face.Indices[2];

                    ++fi;
                    ++i;
                }
            }

            Vertices.erase(Vertices.begin() + index);
        }
    }

    void Mesh::DeleteEdge(int index) {
        if(index < Edges.size()) {
            Edges.erase(Edges.begin() + index);
            --EdgeCount;
        }
    }

    void Mesh::DeleteFace(int index) {
        std::cout << "Deleting face " << index << " from mesh with " << Faces.size() << " faces" << std::endl;
        if(index < Faces.size()) {
            Faces.erase(Faces.begin() + index);
            FaceColors.erase(FaceColors.begin() + index);
            --FaceCount;
        }
    }

    void Mesh::ClearData() {
        Vertices.clear();
        UVs.clear();
        Edges.clear();
        Faces.clear();
        FaceColors.clear();
        VertexCount = 0;
        EdgeCount = 0;
        FaceCount = 0;
    }

    void Mesh::Serialize(Serializer& root) {
        root.WriteInt("VertexCount", VertexCount);
        root.WriteInt("EdgeCount", EdgeCount);
        root.WriteInt("FaceCount", FaceCount);

        // Write verts
        for(int i = 0; i < VertexCount; ++i) {
            std::stringstream ss;
            ss << "Vertex" << i;
            Serializer vert = root.GetChild(ss.str());
            vert.WriteFloat("X", Vertices[i][0]);
            vert.WriteFloat("Y", Vertices[i][1]);
            root.Append(vert, ss.str());
        }

        // Write UVs
        for(int i = 0; i < VertexCount; ++i) {
            std::stringstream ss;
            ss << "UV" << i;
            Serializer uv = root.GetChild(ss.str());
            uv.WriteFloat("U", UVs[i][0]);
            uv.WriteFloat("V", UVs[i][1]);
            root.Append(uv, ss.str());
        }

        // Write edges
        for(int i = 0; i < EdgeCount; ++i) {
            std::stringstream ss;
            ss << "Edge" << i;
            Serializer edge = root.GetChild(ss.str());
            edge.WriteInt("I1", Edges[i].Indices[0]);
            edge.WriteInt("I2", Edges[i].Indices[1]);
            root.Append(edge, ss.str());
        }

        // Write faces
        for(int i = 0; i < FaceCount; ++i) {
            std::stringstream ss;
            ss << "Face" << i;
            Serializer face = root.GetChild(ss.str());
            face.WriteInt("I1", Faces[i].Indices[0]);
            face.WriteInt("I2", Faces[i].Indices[1]);
            face.WriteInt("I3", Faces[i].Indices[2]);
            root.Append(face, ss.str());
        }

        // Write face colors
        for(int i = 0; i < FaceCount; ++i) {
            std::stringstream ss;
            ss << "FaceColor" << i;
            Serializer faceColor = root.GetChild(ss.str());
            faceColor.WriteFloat("R", FaceColors[i].GetR());
            faceColor.WriteFloat("G", FaceColors[i].GetG());
            faceColor.WriteFloat("B", FaceColors[i].GetB());
            faceColor.WriteFloat("A", FaceColors[i].GetA());
            root.Append(faceColor, ss.str());
        }
    }

    void Mesh::Deserialize(Serializer& root) {
        root.ReadInt("VertexCount", VertexCount);
        root.ReadInt("EdgeCount", EdgeCount);
        root.ReadInt("FaceCount", FaceCount);

        /*Vertices = new Point[VertexCount];
        UVs = new Point[VertexCount];
        Edges = new IndexedEdge[EdgeCount];
        Faces = new IndexedFace[FaceCount];
        FaceColors = new Color[FaceCount];*/
        Vertices.resize(VertexCount);
        UVs.resize(VertexCount);
        Edges.resize(EdgeCount);
        Faces.resize(FaceCount);
        FaceColors.resize(FaceCount);

        // read verts
        for(int i = 0; i < VertexCount; ++i) {
            std::stringstream ss;
            ss << "Vertex" << i;
            Serializer vert = root.GetChild(ss.str());
            vert.ReadFloat("X", Vertices[i][0]);
            vert.ReadFloat("Y", Vertices[i][1]);
        }

        // read uvs
        for(int i = 0; i < VertexCount; ++i) {
            std::stringstream ss;
            ss << "UV" << i;
            Serializer uv = root.GetChild(ss.str());
            uv.ReadFloat("U", UVs[i][0]);
            uv.ReadFloat("V", UVs[i][1]);
        }

        // read edges
        for(int i = 0; i < EdgeCount; ++i) {
            std::stringstream ss;
            ss << "Edge" << i;
            Serializer edge = root.GetChild(ss.str());
            edge.ReadInt("I1", Edges[i].Indices[0]);
            edge.ReadInt("I2", Edges[i].Indices[1]);
        }

        // read faces
        for(int i = 0; i < FaceCount; ++i) {
            std::stringstream ss;
            ss << "Face" << i;
            Serializer face = root.GetChild(ss.str());
            face.ReadInt("I1", Faces[i].Indices[0]);
            face.ReadInt("I2", Faces[i].Indices[1]);
            face.ReadInt("I3", Faces[i].Indices[2]);
        }

        // read face colors
        for(int i = 0; i < FaceCount; ++i) {
            std::stringstream ss;
            ss << "FaceColor" << i;
            Serializer faceColor = root.GetChild(ss.str());
            float r, g, b, a;
            faceColor.ReadFloat("R", r);
            faceColor.ReadFloat("G", g);
            faceColor.ReadFloat("B", b);
            faceColor.ReadFloat("A", a);
            FaceColors[i].SetAll(r, g, b, a);
        }
    }

    /*!
        \brief
            Sets the center for transformations

        \param newCenter
            New center for transformations
    */
    void Mesh::SetCenter(const Point& newCenter) {
        Center = newCenter;
    }

    /*!
        \brief
            Gets the center for transformations

        \return
            Center for transformations
    */
    const Point& Mesh::GetCenter() const {
        return Center;
    }

    /*!
        \brief
            Gets the number of vertices in the mesh

        \return
            Number of vertices in the mesh
    */
    int Mesh::GetVertexCount() const {
        return VertexCount;
    }

    /*!
        \brief
            Sets the value for a vertex in the mesh

        \param vertex
            New value of the vertex

        \param index
            Index of the vertex to change
    */
    void Mesh::SetVertex(const Point& vertex, int index) {
        if(index >= VertexCount) {
            throw new Exception("Tried to set vertex out of bounds");
        }

        Vertices[index] = vertex;
    }

    /*!
        \brief
            Gets a vertex from the mesh

        \param index
            Index of the vertex to retrieve

        \return
            Vertex at the given index
    */
    const Point& Mesh::GetVertex(int index) const {
        if(index >= VertexCount) {
            throw new Exception("Tried to get vertex out of bounds");
        }

        return Vertices[index];
    }

    /*!
        \brief
            Sets the value for a vertex in the mesh

        \param vertex
            New value of the vertex

        \param index
            Index of the vertex to change
    */
    void Mesh::SetUV(const Point& uv, int index) {
        if(index >= VertexCount) {
            throw new Exception("Tried to get uv out of bounds");
        }

        UVs[index] = uv;
    }

    /*!
        \brief
            Gets a uv from the mesh

        \param index
            Index of the uv to retrieve

        \return
            uv at the given index
    */
    const Point& Mesh::GetUV(int index) const {
        if(index >= VertexCount) {
            throw new Exception("Tried to get UV out of bounds");
        }

        return UVs[index];
    }

    /*!
        \brief
            Gets the number of edges in the mesh

        \return
            Number of edges in the mesh
    */
    int Mesh::GetEdgeCount() const {
        return EdgeCount;
    }

    /*!
        \brief
            Sets the vertex indices for an edge

        \param edge
            New values for the indexed edge

        \param index
            Index of the edge to change
    */
    void Mesh::SetEdge(const IndexedEdge& edge, int index) {
        if(index >= EdgeCount) {
            throw new Exception("Tried to set edge out of bounds");
        }
        for(int i = 0; i < 2; ++i) {
            if(edge.Indices[i] >= VertexCount) {
                throw new Exception("Edge vertex index out of bounds");
            }
        }

        Edges[index] = edge;
    }

    /*!
        \brief
            Gets an edge in object coordinates from the mesh

        \param index
            Index of the edge to retrieve

        \return
            Edge in object coordinates at given index
    */
    Edge Mesh::GetEdge(int index) const {
        if(index >= EdgeCount) {
            throw new Exception("Tried to get edge out of bounds");
        }

        Point first = Vertices[Edges[index].Indices[0]];
        Point second = Vertices[Edges[index].Indices[1]];
        return Edge(first, second);
    }

    /*!
        \brief
            Gets an indexed edge

        \param index
            Index of the edge to retrieve

        \return
            Indexed edge at given index
    */
    const IndexedEdge& Mesh::GetIndexedEdge(int index) const {
        if(index >= EdgeCount) {
            throw new Exception("Tried to get edge out of bounds");
        }

        return Edges[index];
    }

    /*!
        \brief
            Gets the number of faces in the mesh

        \return
            Number of faces in the mesh
    */
    int Mesh::GetFaceCount() const {
        return FaceCount;
    }

    /*!
        \brief
            Sets the indices for a face

        \param face
            New indices for the face

        \param index
            Index of the face to change
    */
    void Mesh::SetFace(const IndexedFace& face, int index) {
        if(index >= FaceCount) {
            throw new Exception("Tried to set face out of bounds");
        }
        for(int i = 0; i < 3; ++i) {
            if(face.Indices[i] >= VertexCount) {
                throw new Exception("Face vertex index out of bounds");
            }
        }

        Faces[index] = face;
    }

    /*!
        \brief
            Gets a face in object coordinates from the mesh

        \param index
            Index of the face to retrieve

        \return
            Face in object coordinates at the given index
    */
    Face Mesh::GetFace(int index) const {
        if(index >= FaceCount) {
            throw new Exception("Tried to get face out of bounds");
        }

        Point first = Vertices[Faces[index].Indices[0]];
        Point second = Vertices[Faces[index].Indices[1]];
        Point third = Vertices[Faces[index].Indices[2]];

        return Face(first, second, third);
    }

    /*!
        \brief
            Gets an indexed face from the mesh

        \param index
            Index of the face to retrieve

        \return
            Indexed face at the given index
    */
    const IndexedFace& Mesh::GetIndexedFace(int index) const {
        if(index >= FaceCount) {
            throw new Exception("Tried to get face out of bounds");
        }

        return Faces[index];
    }

    /*!
        \brief
            Sets a face color

        \param index
            Index of the face to color

        \param color
            New color of the face
    */
    void Mesh::SetFaceColor(const Color& color, int index) {
        if(index >= FaceCount) {
            throw new Exception("Tried to set face color out of bounds");
        }

        FaceColors[index] = color;
    }

    /*!
        \brief
            Sets a face color

        \param index
            Index of the face to color

        \param r
            Red component of the face

        \param b
            Blue component of the face

        \param g
            Green component of the face

        \param a
            Alpha component of the face
    */
    void Mesh::SetFaceColor(float r, float g, float b, float a, int index) {
        if(index >= FaceCount) {
            throw new Exception("Tried to set face color out of bounds");
        }

        FaceColors[index].SetAll(r, g, b, a);
    }

    /*!
        \brief
            Gets the color of a face

        \param index
            Index of the color to retrieve

        \return
            Color of the face
    */
    Color Mesh::GetFaceColor(int index) const {
        if(index >= FaceCount) {
            throw new Exception("Tried to get face color out of bounds");
        }

        return FaceColors[index];
    }
}
