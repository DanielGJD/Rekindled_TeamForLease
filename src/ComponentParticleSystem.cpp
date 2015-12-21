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

            std::cout << "Particle System: " << 1 / dt << std::endl
                      << "  Total Particles: " << Particles.size() << std::endl
                      << "  Alive Particles: " << ActiveParticles.size() << std::endl
                      << "  Dead Particles:  " << InactiveParticles.size() << std::endl
                      << std::endl;
        }

        void ParticleSystem::Serialize(Serializer& root) {
        }

        void ParticleSystem::Deserialize(Serializer& root) {
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
            // Make all particles inactive
        }

        /*void ParticleSystem::SetMaxParticles(unsigned int maxParticles) {
            // Reset particle system to use new max particles
        }*/
    }
}
