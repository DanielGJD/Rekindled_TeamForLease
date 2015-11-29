#include "ComponentVisionCone.h"
#include "Engine.h" // Just for testing, remove when done
#include "LevelComponentRenderer.h" // Testing
#include <vector>
#include <iostream>

static const int ViewResolution = 12;

namespace ForLeaseEngine {
    namespace Components {
        VisionCone::VisionCone(Entity& parent, bool active, bool drawOutline, bool visible, Vector const& offset, float radius, Vector const& direction, float angle, Color const& indicatorColor)
                              : Component(parent),
                                Active(active), Visible(visible), DrawOutline(drawOutline),
                                Offset(offset), Radius(radius), Direction(direction),
                                Angle(angle), IndicatorColor(indicatorColor) {}

        VisionCone::~VisionCone() {}

        ComponentType VisionCone::GetType() { return Type; }

        void VisionCone::Update() {
            ViewMesh.ClearData();
            Direction.Normalize();
            Vector start = Vector::Rotate(Direction, -Angle / 2);

            // Testing code
            LevelComponents::Renderer* render = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();
            ViewMesh.AddVertex(Offset, Point(0, 0));
            ViewMesh.AddVertex(Point(0, 0) + Offset + start * Radius, Point(0, 0));
            int numPoints = 5;
            for(int i = 1; i < numPoints; ++i) {
                Vector currentDir = Vector::Rotate(start, Angle / (numPoints - 1) * i);
                ViewMesh.AddVertex(Point(0, 0) + Offset + currentDir * Radius, Point(0, 0));
            }

            // Add faces
            for(int i = 2; i < ViewMesh.GetVertexCount(); ++i) {
                ViewMesh.AddFace(IndexedFace(0, i - 1, i), IndicatorColor);
            }

            // Add edges
            for(int i = 1; i < ViewMesh.GetVertexCount(); ++i) {
                ViewMesh.AddEdge(i - 1, i);
            }
            ViewMesh.AddEdge(ViewMesh.GetVertexCount() - 1, 0);

            // Check for observed objects
            Components::Transform* trans = Parent.GetComponent<Components::Transform>();
            std::vector<Entity*> detected = ForLease->GameStateManager().CurrentState().GetEntitiesInCone(trans->Position + Offset, Radius, Direction, Angle);
            std::cout << "Can see " << detected.size() << " entities" << std::endl;
        }

        void VisionCone::Serialize(Serializer& root) {
        }

        void VisionCone::Deserialize(Serializer& root) {
        }

        Mesh* VisionCone::GetVisionMesh() {
            return &ViewMesh;
        }
    }
}
