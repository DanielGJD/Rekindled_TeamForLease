/*!
    \file   LevelComponent.h
    \author Sean McGeer
    \date   9/24/15
    \brief
        Defines the LevelComponent class, which all level components should inherit from.
    \see LevelComponent.cpp
*/

#ifndef LEVEL_COMPONENT_H
#define LEVEL_COMPONENT_H

#include "Component.h"
#include "Entity.h"
#include "Engine.h"
#include "Serializable.h"
#include "Serialize.h"
#include <vector>

namespace ForLeaseEngine {

    class State;

    class LevelComponent : public Serializable {
        public:
            LevelComponent(State& owner, ComponentType componentMask = ComponentType::None);
            virtual void Update(std::vector<Entity *>& entities) = 0;
            bool CheckEntityCompatibility(Entity*  entity);
            virtual void Serialize(Serializer&) {}
            virtual void Deserialize(Serializer&) {}
        protected:
            State& Owner;
        private:
            ComponentType ComponentMask;
    };

} // ForLeaseEngine

#endif
