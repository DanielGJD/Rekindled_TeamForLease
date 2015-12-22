#include "ComponentParticleColorAnimator.h"
#include "ComponentParticleSystem.h"
#include "Interpolation.h"
#include <list>
#include <iostream>

namespace ForLeaseEngine {
    namespace Components {
        ParticleColorAnimator::ParticleColorAnimator(Entity& parent, bool active)
                                                    : Component(parent, ComponentType::ParticleSystem), Active(active), Colors(std::vector<Color>()) {}

        ParticleColorAnimator::~ParticleColorAnimator() {}

        ComponentType ParticleColorAnimator::GetType() { return Type; }

        void ParticleColorAnimator::Update() {
            if(Active && !Colors.empty()) {
                Components::ParticleSystem* system = Parent.GetComponent<Components::ParticleSystem>();
                std::list<Particle*> const* particles = system->GetActiveParticles();

                if(!particles->empty()) {
                    for(std::list<Particle*>::const_iterator i = particles->begin(); i != particles->end(); ++i) {
                        float overallT = (1 - ((*i)->Life / (*i)->StartingLife)) * (Colors.size() - 1);
                        unsigned int index = static_cast<unsigned int>(overallT);
                        float t = overallT - index;

                        if(index + 1 > Colors.size() - 1)
                            //std::cout << "Index outside range: " << index + 1 << std::endl;
                            continue;

                        Color color1 = Colors[index];
                        Color color2 = Colors[index + 1];
                        Color colorMixed = Color(Interpolation::Linear(color1.GetR(), color2.GetR(), t),
                                                 Interpolation::Linear(color1.GetG(), color2.GetG(), t),
                                                 Interpolation::Linear(color1.GetB(), color2.GetB(), t),
                                                 Interpolation::Linear(color1.GetA(), color2.GetA(), t));

                        (*i)->ParticleColor = colorMixed;
                    }
                }
            }
        }

        void ParticleColorAnimator::Serialize(Serializer& root) {
        }

        void ParticleColorAnimator::Deserialize(Serializer& root) {
        }

        void ParticleColorAnimator::AddColor(Color const& color) {
            Colors.push_back(color);
        }

        void ParticleColorAnimator::RemoveColor(unsigned int pos) {
            if(pos < Colors.size()) {
                std::vector<Color>::iterator i = Colors.begin();
                while(pos > 0) {
                    ++i;
                    --pos;
                }
                Colors.erase(i);
            }
        }

        void ParticleColorAnimator::SetColor(unsigned int pos, Color const& color) {
            if(pos < Colors.size())
                Colors[pos] = color;
        }

        Color ParticleColorAnimator::GetColor(unsigned int pos) {
            if(pos < Colors.size())
                return Colors[pos];

            return Color();
        }

        unsigned int ParticleColorAnimator::GetColorCount() {
            return Colors.size();
        }
    }
}
