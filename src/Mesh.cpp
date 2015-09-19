/*!
    \file   Mesh.cpp
    \author Christopher Hudson
    \date   09/19/15

    \brief
        Defines classes to hold data about a triangular face

    \see Mesh.h
*/

#include "Exception.h"
#include "Edge.h"
#include "Face.h"
#include "Mesh.h"

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
        Vertices = new Point[numVerts];
        Edges = new IndexedEdge[numEdges];
        Faces = new IndexedFace[numFaces];
    }

    /*!
        \brief
            Cleans up resources created by the mesh
    */
    Mesh::~Mesh() {
        delete Vertices;
        delete Edges;
        delete Faces;
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

        \param
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
    const IndexedEdge Mesh::GetIndexedEdge(int index) const {
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
}
