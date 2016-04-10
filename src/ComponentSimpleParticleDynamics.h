#ifndef COMPONENT_SIMPLE_PARTICLE_DYNAMICS_H
#define COMPONENT_SIMPLE_PARTICLE_DYNAMICS_H

#include "Component.h"
#include "Vector.h"

namespace ForLeaseEngine {
    namespace Components {
        class SimpleParticleDynamics : public Component {
            public:
                static const ComponentType Type = ComponentType::SimpleParticleDynamics;
                bool Active;
                Vector Force;
                Vector ForceRandom;
                float Torque;
                float TorqueRandom;
                float Growth;
                float GrowthRandom;
                float Drag;
                float DragRandom;

                SimpleParticleDynamics(Entity& parent,
                                       bool active = true,
                                       const Vector& force = Vector(0, 0),
                                       const Vector& forceRandom = Vector(0, 0),
                                       float torque = 0,
                                       float torqueRandom = 0,
                                       float growth = 0,
                                       float growthRandom = 0,
                                       float drag = 0,
                                       float dragRandom = 0);
                ~SimpleParticleDynamics();
                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
            private:
        };
    }
}

#endif // COMPONENT_SIMPLE_PARTICLE_DYNAMICS_H
