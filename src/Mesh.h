/*!
    \file   Mesh.h
    \author Christopher Hudson
    \date   10/03/15

    \brief
        Defines classes to hold data about a triangular face

    \see Mesh.cpp
*/

#ifndef MESH_H
#define MESH_H

#include "Face.h"
#include "Edge.h"
#include "Color.h"
#include "Serializable.h"
#include <vector>

namespace ForLeaseEngine {
    /*!
        \class Mesh

        \brief
            Class that contains mesh data
    */
    class Mesh : public Serializable {
        public:
            Mesh(int numVerts, int numEdges, int numFaces);
            Mesh();
            ~Mesh();

            void Serialize(Serializer& root);
            void Deserialize(Serializer& root);

            void AddVertex(float x, float y, float u, float v);
            void AddVertex(const Point& vertex, const Point& uv);
            void AddEdge(int v1, int v2);
            void AddEdge(const IndexedEdge& edge);
            void AddFace(int v1, int v2, int v3, float r, float g, float b, float a);
            void AddFace(const IndexedFace& face, const Color& color);

            void DeleteVertex(int index);
            void DeleteEdge(int index);
            void DeleteFace(int index);

            void ClearData();

            int GetVertexIndexNear(const Point& selection, float distance = 0.1);

            void SetCenter(const Point& newCenter);
            const Point& GetCenter() const;
            int GetVertexCount() const;
            void SetVertex(const Point& vertex, int index);
            const Point& GetVertex(int index) const;
            void SetUV(const Point& uv, int index);
            const Point& GetUV(int index) const;
            int GetEdgeCount() const;
            void SetEdge(const IndexedEdge& edge, int index);
            Edge GetEdge(int index) const;
            const IndexedEdge& GetIndexedEdge(int index) const;
            int GetFaceCount() const;
            void SetFace(const IndexedFace& face, int index);
            Face GetFace(int index) const;
            const IndexedFace& GetIndexedFace(int index) const;
            void SetFaceColor(const Color& color, int index);
            void SetFaceColor(float r, float g, float b, float a, int index);
            Color GetFaceColor(int index) const;
        private:
            //! Center used for transformations
            Point Center;
            //! Vertex array
            std::vector<Point> Vertices;
            //! UV coordinate array
            std::vector<Point> UVs;
            //! Edge list using indexed edges
            std::vector<IndexedEdge> Edges;
            //! Face list using indexed faces
            std::vector<IndexedFace> Faces;
            //! List of colors for each face
            std::vector<Color> FaceColors;
            //! Number of vertices
            int VertexCount;
            //! Number of edges
            int EdgeCount;
            //! Number of faces
            int FaceCount;

            Mesh(Mesh& original) = delete;
            Mesh& operator=(Mesh& rhs) = delete;
    };
}

#endif // MESH_H
