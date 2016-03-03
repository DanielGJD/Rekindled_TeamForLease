#include "ComponentParticleEmitter.h"
#include "Engine.h"
#include "Random.h"
#include "ComponentParticleSystem.h"
#include <iostream>

namespace ForLeaseEngine {
    namespace Components {
        ParticleEmitter::ParticleEmitter(Entity& parent, bool active, Vector const& emitterSize, Vector const& offset,
                                         unsigned int emitCount, float emitRate, float emitRandom,
                                         float size, float sizeRandom, float life, float lifeRandom,
                                         float rotation, float rotationRandom, Vector const& velocity, Vector const& velocityRandom,
                                         float rotationalVelocity, float rotationalVelocityRandom)
                                        : Component(parent, ComponentType::ParticleSystem), Active(active), EmitterSize(emitterSize), Offset(offset),
                                          EmitCount(emitCount), EmitRate(emitRate), EmitRandom(emitRandom),
                                          Size(size), SizeRandom(sizeRandom), Life(life), LifeRandom(lifeRandom),
                                          Rotation(rotation), RotationRandom(rotationRandom), Velocity(velocity), VelocityRandom(velocityRandom),
                                          RotationalVelocity(rotationalVelocity), RotationalVelocityRandom(rotationalVelocityRandom), EmitTimer(0) {}

        ParticleEmitter::~ParticleEmitter() {}

        ComponentType ParticleEmitter::GetType() { return Type; }

        void ParticleEmitter::Update() {
            if(Active) {
                //std::cout << "ParticleSystem update" << std::endl;
                float dt = ForLease->FrameRateController().GetDt();
                EmitTimer -= dt;

                if(EmitTimer <= 0) {
                    EmitParticles(EmitCount);
                    EmitTimer = 1 / (EmitRate + RandomFloat(-EmitRandom, EmitRandom));
                }
            }
        }

        void ParticleEmitter::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer emitter = root.GetChild("ParticleEmitter");
            emitter.WriteUint("Type", static_cast<unsigned int>(Type));
            emitter.WriteBool("Active", Active);
            emitter.WriteVec("EmitterSize", EmitterSize);
            emitter.WriteVec("Offset", Offset);
            emitter.WriteUint("EmitCount", EmitCount);
            emitter.WriteFloat("EmitRate", EmitRate);
            emitter.WriteFloat("EmitRandom", EmitRandom);
            emitter.WriteFloat("Size", Size);
            emitter.WriteFloat("SizeRandom", SizeRandom);
            emitter.WriteFloat("Life", Life);
            emitter.WriteFloat("LifeRandom", LifeRandom);
            emitter.WriteFloat("Rotation", Rotation);
            emitter.WriteFloat("RotationRandom", RotationRandom);
            emitter.WriteVec("Velocity", Velocity);
            emitter.WriteVec("VelocityRandom", VelocityRandom);
            emitter.WriteFloat("RotationalVelocity", RotationalVelocity);
            emitter.WriteFloat("RotationalVelocityRandom", RotationalVelocityRandom);
            root.Append(emitter, "ParticleEmitter");
        }

        void ParticleEmitter::Deserialize(Serializer& root) {
            Serializer emitter = root.GetChild("ParticleEmitter");
            emitter.ReadBool("Active", Active);
            emitter.ReadVec("EmitterSize", EmitterSize);
            emitter.ReadVec("Offset", Offset);
            emitter.ReadUint("EmitCount", EmitCount);
            emitter.ReadFloat("EmitRate", EmitRate);
            emitter.ReadFloat("EmitRandom", EmitRandom);
            emitter.ReadFloat("Size", Size);
            emitter.ReadFloat("SizeRandom", SizeRandom);
            emitter.ReadFloat("Life", Life);
            emitter.ReadFloat("LifeRandom", LifeRandom);
            emitter.ReadFloat("Rotation", Rotation);
            emitter.ReadFloat("RotationRandom", RotationRandom);
            emitter.ReadVec("Velocity", Velocity);
            emitter.ReadVec("VelocityRandom", VelocityRandom);
            emitter.ReadFloat("RotationalVelocity", RotationalVelocity);
            emitter.ReadFloat("RotationalVelocityRandom", RotationalVelocityRandom);
        }

        void ParticleEmitter::EmitParticles(unsigned int count) {
            Components::ParticleSystem* system = Parent.GetComponent<Components::ParticleSystem>();
            Components::Transform* trans = Parent.GetComponent<Components::Transform>();

            for(unsigned int i = 0; i < count; ++i) {
                Particle particle = Particle(Life + RandomFloat(-LifeRandom, LifeRandom),
                                             Point(trans->Position[0] + RandomFloat(-EmitterSize[0], EmitterSize[0]) + Offset[0], trans->Position[1] + RandomFloat(-EmitterSize[1], EmitterSize[1]) + Offset[1]),
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
