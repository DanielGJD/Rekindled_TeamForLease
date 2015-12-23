#include "ComponentParticleSystem.h"
#include "Engine.h"
#include <iostream>

namespace ForLeaseEngine {
    namespace Components {
        ParticleSystem::ParticleSystem(Entity& parent,
                                       bool visible,
                                       BlendMode blendMode,
                                       Color startingColor,
                                       unsigned int maxParticles,
                                       Vector systemSize)
                                       : Component(parent, ComponentType::Transform),
                                         Visible(visible),
                                         BlendingMode(blendMode),
                                         StartingColor(startingColor),
                                         MaxParticles(maxParticles),
                                         SystemSize(systemSize),
                                         Particles(std::vector<Particle>(maxParticles)),
                                         ActiveParticles(std::list<Particle*>()),
                                         InactiveParticles(std::list<Particle*>()) {
            for(unsigned int i = 0; i < Particles.size(); ++i) {
                InactiveParticles.push_front(&Particles[i]);
            }
        }

        ParticleSystem::~ParticleSystem() {}

        ComponentType ParticleSystem::GetType() { return Type; }

        void ParticleSystem::Update() {
            // Update positions, life, remove dead particles
            double dt = ForLease->FrameRateController().GetDt();

            for(std::list<Particle*>::iterator i = ActiveParticles.begin(); i != ActiveParticles.end(); ++i) {
                (*i)->Life -= dt;
                if((*i)->Life <= 0) {
                    InactiveParticles.push_front((*i));
                    i = ActiveParticles.erase(i);
                    if(i == ActiveParticles.end())
                        break;
                }
                else {
                    (*i)->Position += Vector::Scale((*i)->Velocity, dt);
                    (*i)->Rotation += (*i)->RotationalVelocity * dt;
                }
            }

            /*std::cout << "Particle System: " << 1 / dt << std::endl
                      << "  Total Particles: " << Particles.size() << std::endl
                      << "  Alive Particles: " << ActiveParticles.size() << std::endl
                      << "  Dead Particles:  " << InactiveParticles.size() << std::endl
                      << std::endl;*/
        }

        void ParticleSystem::Serialize(Serializer& root) {
            Serializer system = root.GetChild("ParticleSystem");
            system.WriteUint("Type", static_cast<unsigned int>(Type));
            system.WriteBool("Visible", Visible);
            system.WriteInt("BlendingMode", BlendingMode);
            StartingColor.Serialize(system);
            system.WriteUint("MaxParticles", MaxParticles);
            system.WriteVec("SystemSize", SystemSize);
            root.Append(system, "ParticleSystem");
        }

        void ParticleSystem::Deserialize(Serializer& root) {
            Serializer system = root.GetChild("ParticleSystem");
            system.ReadBool("Visible", Visible);
            int mode;
            system.ReadInt("BlendingMode", mode);
            BlendingMode = static_cast<BlendMode>(mode);
            StartingColor.Deserialize(system);
            system.ReadUint("MaxParticles", MaxParticles);
            system.ReadVec("SystemSize", SystemSize);
        }

        std::vector<Particle> const* ParticleSystem::GetAllParticles() const{
            return &Particles;
        }

        std::list<Particle*> const* ParticleSystem::GetActiveParticles() const{
            return &ActiveParticles;
        }

        std::list<Particle*> const* ParticleSystem::GetInactiveParticles() const{
            return &InactiveParticles;
        }

        void ParticleSystem::CreateParticle(Particle const& newParticle) {
            // Create particles from inactive particles
            if(InactiveParticles.size() == 0)
                return;

            Particle* particle = InactiveParticles.front();
            InactiveParticles.pop_front();
            *particle = newParticle;
            ActiveParticles.push_back(particle);
        }

        void ParticleSystem::KillAllParticles() {
            while(!ActiveParticles.empty()) {
                InactiveParticles.push_back(ActiveParticles.front());
                ActiveParticles.pop_front();
            }
        }

        void ParticleSystem::SetMaxParticles(unsigned int maxParticles) {
            ActiveParticles.clear();
            InactiveParticles.clear();
            Particles.reserve(maxParticles);
            for(unsigned int i = 0; i < Particles.size(); ++i)
                InactiveParticles.push_back(&Particles[i]);
        }
    }
}
