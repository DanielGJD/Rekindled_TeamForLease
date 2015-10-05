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
#include <vector>

namespace ForLeaseEngine {

    class Engine;

    class LevelComponent {
        public:
            LevelComponent(Engine& owner, ComponentType componentMask = ComponentType::None);
            virtual void Update(std::vector<Entity *>& entities) = 0;
            bool CheckEntityCompatibility(Entity*  entity);
        private:
            Engine& Owner;
            ComponentType ComponentMask;
    };

} // ForLeaseEngine

#endif
