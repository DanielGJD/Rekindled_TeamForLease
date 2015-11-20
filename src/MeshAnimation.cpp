#include "MeshAnimation.h"
#include "Exception.h"
#include "Interpolation.h"
#include <iostream>
#include <sstream>

namespace ForLeaseEngine {
    MeshAnimation::MeshAnimation() {}

    MeshAnimation::MeshAnimation(Mesh const* mesh, std::string name)
                                : AnimationName(name) {
        std::cout << "Creating animation from mesh with " << mesh->GetVertexCount() << " vertices" << std::endl;
        FrameVertexData.push_back(std::vector<Point>());
        for(unsigned int i = 0; i < static_cast<unsigned int>(mesh->GetVertexCount()); ++i) {
            FrameVertexData[0].push_back(mesh->GetVertex(i));
        }
    }

    void MeshAnimation::SetAnimationName(std::string const& name) {
        AnimationName = name;
    }

    std::string const& MeshAnimation::GetAnimationName() {
        return AnimationName;
    }

    void MeshAnimation::Serialize(Serializer& root) {
        Serializer animation = root.GetChild("MeshAnimation");
        animation.WriteString("AnimationName", AnimationName);
        animation.WriteUint("FrameCount", FrameVertexData.size());
        if(FrameVertexData.size() > 0) {
            animation.WriteUint("VertexCount", FrameVertexData[0].size());
        }
        else {
            animation.WriteString("VertexCount", 0);
        }

        Serializer frames = animation.GetChild("Frames");
        for(unsigned int i = 0; i < FrameVertexData.size(); ++i) {
            std::vector<Point> f = FrameVertexData[i];
            std::stringstream frameNumber;
            frameNumber << "Frame" << i;
            Serializer frame = frames.GetChild(frameNumber.str());
            for(unsigned int j = 0; j < FrameVertexData[0].size(); ++j) {
                std::stringstream vertexNumber;
                vertexNumber << "Vertex" << j;
                Serializer vertex = frame.GetChild(vertexNumber.str());
                Point p = f[j];
                vertex.WriteFloat("X", p[0]);
                vertex.WriteFloat("Y", p[1]);
                frame.Append(vertex, vertexNumber.str());
            }
            frames.Append(frame, frameNumber.str());
        }
        animation.Append(frames, "Frames");
        root.Append(animation, "MeshAnimation");
    }

    void MeshAnimation::Deserialize(Serializer& root) {
        Serializer animation = root.GetChild("MeshAnimation");
        animation.ReadString("AnimationName", AnimationName);
        unsigned int frameCount;
        animation.ReadUint("FrameCount", frameCount);
        unsigned int vertexCount;
        animation.ReadUint("VertexCount", vertexCount);

        Serializer frames = animation.GetChild("Frames");
        for(unsigned int i = 0; i < frameCount; ++i) {
            FrameVertexData.push_back(std::vector<Point>());
            std::stringstream frameNumber;
            frameNumber << "Frame" << i;
            std::cout << "Loading " << frameNumber.str() << std::endl;
            Serializer frame = frames.GetChild(frameNumber.str());
            for(unsigned int j = 0; j < vertexCount; ++j) {
                std::stringstream vertexNumber;
                vertexNumber << "Vertex" << j;
                std::cout << "  Loading " << vertexNumber.str() << std::endl;
                Serializer vertex = frame.GetChild(vertexNumber.str());
                float x;
                float y;
                vertex.ReadFloat("X", x);
                vertex.ReadFloat("Y", y);
                FrameVertexData[i].push_back(Point(x, y));
            }
        }

        std::cout << "Loaded animation with " << FrameVertexData.size() << " frames with " << FrameVertexData[0].size() << " vertices" << std::endl;
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
