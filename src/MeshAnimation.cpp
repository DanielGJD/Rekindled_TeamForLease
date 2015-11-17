#include "MeshAnimation.h"
#include "Exception.h"
#include "Interpolation.h"
#include <iostream>

namespace ForLeaseEngine {
    MeshAnimation::MeshAnimation(Mesh const* mesh, std::string name)
                                : AnimationName(name) {
        std::cout << "Creating animation from mesh with " << mesh->GetVertexCount() << " vertices" << std::endl;
        FrameVertexData.push_back(std::vector<Point>());
        for(unsigned int i = 0; i < static_cast<unsigned int>(mesh->GetVertexCount()); ++i) {
            FrameVertexData[0].push_back(mesh->GetVertex(i));
        }
    }

    void MeshAnimation::AddFrame() {
        FrameVertexData.push_back(std::vector<Point>(FrameVertexData[FrameVertexData.size() - 1]));
    }

    void MeshAnimation::InsertFrameBefore(unsigned int position) {
    }

    void MeshAnimation::InsertFrameAfter(unsigned int position) {
    }

    void MeshAnimation::DeleteFrame(unsigned int frame) {
    }

    unsigned int MeshAnimation::GetFrameCount() const {
        return FrameVertexData.size();
    }

    void MeshAnimation::AddVertexToFrames(unsigned int index, Point const& vertex) {
        if(FrameVertexData.size() > 0) {
            if(index >= FrameVertexData[0].size()) {
                throw new Exception("MeshAnimation: Accessed vertex out of bounds");
            }
        }
    }

    void MeshAnimation::DeleteVertexFromFrames(unsigned int index) {
        if(FrameVertexData.size() > 0) {
            if(index >= FrameVertexData[0].size()) {
                throw new Exception("MeshAnimation: Accessed vertex out of bounds");
            }
        }
    }

    void MeshAnimation::SetVertex(unsigned int frame, unsigned int index, Point const& vertex) {
        if(frame >= FrameVertexData.size()) {
            throw new Exception("MeshAnimation: Accessed frame out of bounds");
        }
        if(index >= FrameVertexData[0].size()) {
            throw new Exception("MeshAnimation: Accessed vertex out of bounds");
        }

        FrameVertexData[frame][index] = vertex;
    }

    unsigned int MeshAnimation::GetFrameVertexCount() const {
        if(FrameVertexData.size() > 0) {
            return FrameVertexData[0].size();
        }

        return 0;
    }

    Point const& MeshAnimation::GetVertex(unsigned int frame, unsigned int index) const {
        if(frame >= FrameVertexData.size()) {
            throw new Exception("MeshAnimation: Accessed frame out of bounds");
        }
        if(index >= FrameVertexData[0].size()) {
            throw new Exception("MeshAnimation: Accessed vertex out of bounds");
        }

        return FrameVertexData[frame][index];
    }

    Point MeshAnimation::InterpolateVertex(unsigned int frame, unsigned int index, float t) const {
        if(frame >= FrameVertexData.size()) {
            throw new Exception("MeshAnimation: Accessed frame out of bounds");
        }
        if(index >= FrameVertexData[0].size()) {
            throw new Exception("MeshAnimation: Accessed vertex out of bounds");
        }

        return Point(Interpolation::Linear(FrameVertexData[frame][index][0], FrameVertexData[(frame + 1) % GetFrameCount()][index][0], t),
                     Interpolation::Linear(FrameVertexData[frame][index][1], FrameVertexData[(frame + 1) % GetFrameCount()][index][1], t));
    }
}
