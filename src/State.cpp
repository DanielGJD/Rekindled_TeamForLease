/*!
    \file   State.cpp
    \author Sean McGeer
    \date   9/17/15
    \brief
        Contains implementations for the State class defined in State.h.
    \see State.h
*/

#include "State.h"
#include "Engine.h"

namespace ForLeaseEngine {

    /*!
        Constructor for a State.  Initializes Entities.

        \param name
            A std::string name to give this state.
    */
    State::State(std::string name) : Name(name), Entities() {}

    /*!
        Destructor for a State.  Deletes all entities.
    */
    State::~State() {
        for (Entity* entity : Entities)
            delete entity;
    }

    void State::UpdateEntities() {
        for (LevelComponent* levelComponent : LevelComponents)
            levelComponent->Update(Entities);
    }

    void State::AddLevelComponent(LevelComponent* levelComponent) {
        LevelComponents.push_back(levelComponent);
    }

    /*!
        Adds an entity to the Entities vector in the state.

        \return
            A pointer to the newly-created entity.
    */
    Entity* State::AddEntity() {
        Entity* entity = new Entity();
        Entities.push_back(entity);

        return entity;
    }

    /*!
        Get a pointer to an entity with the given ID.

        \param id
            The ID of the entity to return.
        \param throwOnFail
            Whether to throw an error on fail, or return a null pointer.
            Defaults to false.
        \return
            A pointer to an entity with the given ID.
    */
    Entity* State::GetEntityByID(long unsigned id, bool throwOnFail) {
        for (Entity* entity : Entities) {

            if (entity->GetID() == id)
                return entity;
        }

        if (throwOnFail) throw EntityNotFoundException(id);

        else return 0;
    }

    /*!
        Getter for the name of a state.

        \return
            The std::string name.
    */
    std::string State::GetName() { return Name; }

    /*!
        Add a level component (or multiple level components) to a given state.

        \param mask
            The mask of ComponentTypes for the LevelComponents we want to add.

        \param state
            The state to add the level component to.
    */
    void AddLevelComponentsToState(ComponentType mask, State* state) {
        if (mask == ComponentType::None)
            return;

        if ((mask & ComponentType::Physics) == ComponentType::Physics)
            state->AddLevelComponent(new LevelComponents::Physics(*state));
        if ((mask & ComponentType::Collision) == ComponentType::Collision)
            state->AddLevelComponent(new LevelComponents::Collision(*state));
    }

}
