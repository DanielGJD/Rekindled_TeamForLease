#ifndef MESH_ANIMATION_H
#define MESH_ANIMATION_H

#include "Mesh.h"
#include "Vector.h"
#include <string>
#include <vector>

namespace ForLeaseEngine {
    class MeshAnimation {
        public:
            MeshAnimation(Mesh const* mesh, std::string name);
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
