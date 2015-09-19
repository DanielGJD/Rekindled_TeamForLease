/*!
    \file   Mesh.h
    \author Christopher Hudson
    \date   09/19/15

    \brief
        Defines classes to hold data about a triangular face

    \see Mesh.cpp
*/

#ifndef MESH_H
#define MESH_H

#include "Face.h"
#include "Edge.h"

namespace ForLeaseEngine {
    class Mesh {
        public:
            Mesh(int numVerts, int numEdges, int numFaces);
            ~Mesh();

            void SetCenter(const Point& newCenter);
            const Point& GetCenter() const;
            int GetVertexCount() const;
            void SetVertex(const Point& vertex, int index);
            const Point& GetVertex(int index) const;
            int GetEdgeCount() const;
            void SetEdge(const IndexedEdge& edge, int index);
            Edge GetEdge(int index) const;
            const IndexedEdge& GetIndexedEdge(int index) const;
            int GetFaceCount() const;
            void SetFace(const IndexedFace& face, int index);
            Face GetFace(int index) const;
            const IndexedFace& GetIndexedFace(int index) const;
        private:
            //! Center used for transformations
            Point Center;
            //! Vertex array
            Point* Vertices;
            //! Edge list using indexed edges
            IndexedEdge* Edges;
            //! Face list using indexed faces
            IndexedFace* Faces;
            //! Number of vertices
            int VertexCount;
            //! Number of edges
            int EdgeCount;
            //! Number of faces
            int FaceCount;
    };
}

#endif // MESH_H
