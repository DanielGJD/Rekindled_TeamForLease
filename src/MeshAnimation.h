/*!
    \file   MeshAnimation.h
    \author Christopher Hudson

    \brief
        Defines a class containing vertex data for a mesh animation

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef MESH_ANIMATION_H
#define MESH_ANIMATION_H

#include "Mesh.h"
#include "Vector.h"
#include "Serializable.h"
#include <string>
#include <vector>

namespace ForLeaseEngine {
    class MeshAnimation : public Serializable {
        public:
            MeshAnimation();
            MeshAnimation(Mesh const* mesh, std::string name);
            void Serialize(Serializer& root);
            void Deserialize(Serializer& root);

            void SetAnimationName(std::string const& name);
            std::string const& GetAnimationName();

            void AddFrame();
            void InsertFrameBefore(unsigned int position);
            void InsertFrameAfter(unsigned int position);
            void DeleteFrame(unsigned int frame);
            unsigned int GetFrameCount() const;
            void AddVertexToFrames(unsigned int index, Point const& vertex);
            void DeleteVertexFromFrames(unsigned int index);
            void SetVertex(unsigned int frame, unsigned int index, Point const& vertex);
            unsigned int GetFrameVertexCount() const;
            Point const& GetVertex(unsigned int frame, unsigned int index) const;
            Point InterpolateVertex(unsigned int frame, unsigned int index, float t) const;
        private:
            std::string AnimationName;
            //int FrameCount;
            //int FrameVertexCount;
            //std::string MeshName;
            std::vector<std::vector<Point>> FrameVertexData;
    };
}

#endif // MESH_ANIMATION_H
