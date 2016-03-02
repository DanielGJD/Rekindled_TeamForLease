#ifndef COMPONENT_PARTICLE_EMITTER_H
#define COMPONENT_PARTICLE_EMITTER_H

#include "Component.h"
#include "Entity.h"

namespace ForLeaseEngine {
    namespace Components {
        class ParticleEmitter : public Component {
            public:
                static const ComponentType Type = ComponentType::ParticleEmitter;
                bool Active;
                Vector EmitterSize;
                Vector Offset;
                unsigned int EmitCount;
                float EmitRate;
                float EmitRandom;
                float Size;
                float SizeRandom;
                float Life;
                float LifeRandom;
                float Rotation;
                float RotationRandom;
                Vector Velocity;
                Vector VelocityRandom;
                float RotationalVelocity;
                float RotationalVelocityRandom;

                ParticleEmitter(Entity& parent,
                                bool active = true,
                                Vector const& emitterSize = Vector(5, 5),
                                Vector const& offset = Vector(0, 0),
                                unsigned int emitCount = 1,
                                float emitRate = 0.25,
                                float emitRandom = 0,
                                float size = 1,
                                float sizeRandom = 0,
                                float life = 1,
                                float lifeRandom = 0,
                                float rotation = 0,
                                float rotationRandom = 0,
                                Vector const& velocity = Vector(0, 0),
                                Vector const& velocityRandom = Vector(0, 0),
                                float rotationalVelocity = 0,
                                float rotationalVelocityRandom = 0);
                ~ParticleEmitter();
                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void EmitParticles(unsigned int count);
            private:
                float EmitTimer;
        };
    }
}

#endif // COMPONENT_PARTICLE_EMITTER_H
