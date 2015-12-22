#ifndef COMPONENT_PARTICLE_SYSTEM_H
#define COMPONENT_PARTICLE_SYSTEM_H

#include "Component.h"
#include "Vector.h"
#include "Color.h"
#include "Entity.h"
#include <list>
#include <vector>

namespace ForLeaseEngine {
    struct Particle {
        float Life;
        Point Position;
        float Size;
        float Rotation;
        Color ParticleColor;
        Vector Velocity;
        float RotationalVelocity;
        float StartingLife;

        Particle(float life = 0,
                Point const& position = Point(0, 0),
                 float size = 0,
                 float rotation = 0,
                 Color const& particleColor = Color(1, 1, 1, 1),
                 Vector const& velocity = Vector(0, 0),
                 float rotationalVelocity = 0)
                : Life(life), Position(position), Size(size), Rotation(rotation), ParticleColor(particleColor), Velocity(velocity), RotationalVelocity(rotationalVelocity), StartingLife(life) {}

        bool operator<(Particle const& rhs) {
            if(Life < rhs.Life)
                return true;
            else
                return false;
        }
    };

    namespace Components {
        class ParticleSystem : public Component {
            public:
                static const ComponentType Type = ComponentType::ParticleSystem;
                bool Visible;
                BlendMode BlendingMode;
                Color StartingColor;
                unsigned int MaxParticles;
                Vector SystemSize;

                ParticleSystem(Entity& parent,
                               bool visible = true,
                               BlendMode blendMode = BlendMode::ALPHA,
                               Color startingColor = Color(1, 1, 1, 1),
                               unsigned int maxParticles = 100,
                               Vector systemSize = Vector(5, 5));
                ~ParticleSystem();
                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                std::vector<Particle> const* GetAllParticles() const;
                std::list<Particle*> const* GetActiveParticles() const;
                std::list<Particle*> const* GetInactiveParticles() const;
                void CreateParticle(Particle const& newParticle);
                void KillAllParticles();
                void SetMaxParticles(unsigned int maxParticles); // Used to reset system to use the new max
            private:
                std::vector<Particle> Particles;
                std::list<Particle*> ActiveParticles;
                std::list<Particle*> InactiveParticles;
        };
    }
}

#endif // COMPONENT_PARTICLE_SYSTEM_H
