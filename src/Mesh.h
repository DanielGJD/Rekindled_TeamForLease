/*!
    \file   Mesh.h
    \author Christopher Hudson
    \date   10/03/15

    \brief
        Defines classes to hold data about a triangular face

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef MESH_H
#define MESH_H

#include "Face.h"
#include "Edge.h"
#include "Color.h"
#include "Serializable.h"
//#include "MeshAnimation.h"
#include <vector>
#include <unordered_set>
#include <string>

namespace ForLeaseEngine {
    class MeshAnimation;
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
            void AddEdge(int v1, int v2, unsigned int order = 0);
            void AddEdge(const IndexedEdge& edge, unsigned int order = 0);
            void AddFace(int v1, int v2, int v3, float r, float g, float b, float a);
            void AddFace(const IndexedFace& face, const Color& color);

            void DeleteVertex(int index);
            void DeleteEdge(int index);
            void DeleteFace(int index);

            void ClearData();

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
            void SetEdgeDrawOrder(unsigned int order, unsigned int index);
            unsigned int GetEdgeDrawOrder(unsigned int index) const;
            int GetFaceCount() const;
            void SetFace(const IndexedFace& face, int index);
            Face GetFace(int index) const;

            const IndexedFace& GetIndexedFace(int index) const;
            void SetFaceColor(const Color& color, int index);
            void SetFaceColor(float r, float g, float b, float a, int index);
            Color GetFaceColor(int index) const;

            int GetVertexIndexNear(const Point& location, std::vector<Point>* vertexData = NULL, float distance = 0.03);
            int GetEdgeIndexNear(const Point& location, std::vector<Point>* vertexData = NULL, float distance = 0.03);
            int GetFaceIndexAt(const Point& location, std::vector<Point>* vertexData = NULL);

            void AddAnimation(std::string const& name);
            void RemoveAnimation(std::string const& name);
            std::vector<std::string> GetAnimationNames();
            std::vector<Point>* GetRawVertexData();
        private:
            //! Center used for transformations
            Point Center;
            //! Vertex array
            std::vector<Point> Vertices;
            //! UV coordinate array
            std::vector<Point> UVs;
            //! Edge list using indexed edges
            std::vector<IndexedEdge> Edges;
            //! Edge draw order
            std::vector<unsigned int> EdgesDrawOrder;
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
            //! Animations
            std::unordered_set<std::string> Animations;

            Mesh(Mesh& original) = delete;
            Mesh& operator=(Mesh& rhs) = delete;
    };
}

#endif // MESH_H
