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
            Point* Vertices;
            //! UV coordinate array
            Point* UVs;
            //! Edge list using indexed edges
            IndexedEdge* Edges;
            //! Face list using indexed faces
            IndexedFace* Faces;
            //! List of colors for each face
            Color* FaceColors;
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
