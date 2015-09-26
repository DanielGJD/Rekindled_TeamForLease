/*!
    \file   System.h
    \author Sean McGeer
    \date   9/24/15
    \brief
        Defines the System class, which all systems should inherit from.
    \see System.cpp
*/

#ifndef SYSTEM_H
#define SYSTEM_H

#include "Component.h"
#include "Entity.h"
#include <vector>

namespace ForLeaseEngine {

    class Engine;

    class System {
        public:
            System(Engine& owner, ComponentType componentMask = ComponentType::None);
            virtual void Update(std::vector<Entity *>& entities) = 0;
            bool CheckEntityCompatibility(Entity*  entity);
        private:
            Engine& Owner;
            ComponentType ComponentMask;
    };

} // ForLeaseEngine

#endif
