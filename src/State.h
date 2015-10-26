/*!
    \file   State.h
    \author Sean McGeer
    \date   9/17/15
    \brief
        Defines the State class.
    \see State.cpp
*/

#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include "Entity.h"
#include "StateExceptions.h"
#include "LevelComponentsInclude.h"
#include "Component.h"

namespace ForLeaseEngine {

    class Engine;

    /*!
        \class State
        \brief
            A small class that defines a state to be loaded by the GameStateManager.
    */
    class State : public Serializable {

        public:
            State(std::string name = "Generic State Name");
            ~State();
            virtual void Load() = 0;
            virtual void Initialize() = 0;
            virtual void Update() = 0;
            virtual void Deinitialize() = 0;
            virtual void Unload() = 0;

            void UpdateEntities();
            void AddLevelComponent(LevelComponent* levelComponent);
            Entity* AddEntity();
            Entity* GetEntityByID(long unsigned id, bool throwOnFail = false);

            std::string GetName();

            virtual void Serialize(Serializer& root);
            virtual void Deserialize(Serializer& root);

        protected:
            std::string Name;                               //! Name of the state
            std::vector<LevelComponent*> LevelComponents;   //! The level components in the state
            std::vector<Entity*> Entities;                  //! The entities in the state

    };

    void AddLevelComponentsToState(ComponentType mask, State* state);

} // ForLeaseEngine

#endif
