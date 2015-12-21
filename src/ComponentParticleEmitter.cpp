#include "ComponentParticleEmitter.h"
#include "Engine.h"
#include "Random.h"
#include "ComponentParticleSystem.h"
#include <iostream>

namespace ForLeaseEngine {
    namespace Components {
        ParticleEmitter::ParticleEmitter(Entity& parent, bool active, Vector const& emitterSize,
                                         unsigned int emitCount, float emitRate, float emitRandom,
                                         float size, float sizeRandom, float life, float lifeRandom,
                                         float rotation, float rotationRandom, Vector const& velocity, Vector const& velocityRandom,
                                         float rotationalVelocity, float rotationalVelocityRandom)
                                        : Component(parent, ComponentType::ParticleSystem), Active(active), EmitterSize(emitterSize),
                                          EmitCount(emitCount), EmitRate(emitRate), EmitRandom(emitRandom),
                                          Size(size), SizeRandom(sizeRandom), Life(life), LifeRandom(lifeRandom),
                                          Rotation(rotation), RotationRandom(rotationRandom), Velocity(velocity), VelocityRandom(velocityRandom),
                                          RotationalVelocity(rotationalVelocity), RotationalVelocityRandom(rotationalVelocityRandom), EmitTimer(0) {}

        ParticleEmitter::~ParticleEmitter() {}

        ComponentType ParticleEmitter::GetType() { return Type; }

        void ParticleEmitter::Update() {
            if(Active) {
                std::cout << "ParticleSystem update" << std::endl;
                float dt = ForLease->FrameRateController().GetDt();
                EmitTimer -= dt;

                if(EmitTimer <= 0) {
                    EmitParticles(EmitCount);
                    EmitTimer = 1 / (EmitRate + RandomFloat(-EmitRandom, EmitRandom));
                }
            }
        }

        void ParticleEmitter::Serialize(Serializer& root) {
        }

        void ParticleEmitter::Deserialize(Serializer& root) {
        }

        void ParticleEmitter::EmitParticles(unsigned int count) {
            Components::ParticleSystem* system = Parent.GetComponent<Components::ParticleSystem>();
            Components::Transform* trans = Parent.GetComponent<Components::Transform>();

            for(unsigned int i = 0; i < count; ++i) {
                Particle particle = Particle(Life + RandomFloat(-LifeRandom, LifeRandom),
                                             Point(trans->Position[0] + RandomFloat(-EmitterSize[0], EmitterSize[0]), trans->Position[1] + RandomFloat(-EmitterSize[1], EmitterSize[1])),
                                             Size + RandomFloat(-SizeRandom, SizeRandom),
                                             Rotation + RandomFloat(-RotationRandom, RotationRandom),
                                             system->StartingColor,
                                             Velocity + Vector(RandomFloat(-VelocityRandom[0], VelocityRandom[0]), RandomFloat(-VelocityRandom[1], VelocityRandom[1])),
                                             RotationalVelocity + RandomFloat(-RotationalVelocityRandom, RotationalVelocityRandom));
                system->CreateParticle(particle);
            }
        }
    }
}
