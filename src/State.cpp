/*!
    \file   State.cpp
    \author Sean McGeer
    \date   9/17/15
    \brief
        Contains implementations for the State class defined in State.h.
    \see State.h

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "State.h"
#include "Engine.h"
#include <sstream>

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
        for (Entity* entity : Entities)
            entity->Update();
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
    Entity* State::AddEntity(std::string name) {
        Entity* entity = new Entity(name);
        Entities.push_back(entity);

        return entity;
    }

    /*!
        Spawns an archetype.

        \param filename
            A std::string filename of the archetype in question.

        \param position
            The point we want to spawn the archetype at.

        \param name
            The name we want to give to the newly created entity.
    */
    Entity* State::SpawnArchetype(std::string filename, Point position, std::string name) {
        Serializer archetype;
        archetype.ReadFile(filename);
        Entity* entity = new Entity();
        std::string name_backup = entity->GetName();
        entity->Deserialize(archetype);

        if (entity->HasComponent(ComponentType::Transform)) {
            Components::Transform* transform = entity->GetComponent<Components::Transform>();
            transform->Position = position;
        }

        if (name != "") entity->SetName(name);
        else entity->SetName(name_backup);

        Entities.push_back(entity);

        return entity;
    }

    bool State::DeleteEntity(long unsigned id) {
        for (unsigned i = 0; i < Entities.size(); ++i) {
            if (Entities[i]->GetID() == id) {
                delete Entities[i];
                Entities.erase(Entities.begin() + i);
                return true;
            }
        }

        return false;
    }

    bool State::DeleteEntity(std::string name) {
        for (unsigned i = 0; i < Entities.size(); ++i) {
            if (Entities[i]->GetName() == name) {
                delete Entities[i];
                Entities.erase(Entities.begin() + i);
                return true;
            }
        }

        return false;
    }

    bool State::DeleteEntity(Entity* entity) {
        for (unsigned i = 0; i < Entities.size(); ++i) {
            if (Entities[i] == entity) {
                delete entity;
                Entities.erase(Entities.begin() + i);
                return true;
            }
        }

        return false;
    }

    void State::DeleteAllEntities() {
        for (Entity* entity : Entities) {
            delete entity;
        }

        Entities.clear();
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
        Get a pointer to an entity with the given name.

        \param name
            A std::string name of the Entity we want to find.

        \param throwOnFail
            Whether to throw an error on fail, or return a null pointer.
            Defaults to false.

        \return
            A pointer to an entity with the given name.
    */
    Entity* State::GetEntityByName(std::string name, bool throwOnFail) {
        for (Entity* entity : Entities) {
            if (entity->GetName() == name)
                return entity;
        }

        std::stringstream ss;
        ss << "Entity " << name << " not found.";


        if (throwOnFail) throw EntityNotFoundException(0, ss.str());

        else return 0;

    }

    Entity* State::GetEntityAtPosition(Point position, bool throwOnFail) {
        for (Entity* entity : Entities) {
            if (!entity->HasComponent(ComponentType::Transform)) continue;

            Components::Transform* transform = entity->GetComponent<Components::Transform>(true);
            
            if (position[0] > transform->Position[0] - transform->ScaleX &&
                position[0] < transform->Position[0] + transform->ScaleX &&
                position[1] > transform->Position[1] - transform->ScaleY &&
                position[1] < transform->Position[1] + transform->ScaleY)
                return entity;
        }

        if (throwOnFail) {
            std::stringstream ss;
            ss << "No entity found at point " << position << ".";

            throw EntityNotFoundException(0, ss.str());
        }

        else return 0;
    }

    std::vector<Entity *> State::GetEntitiesAtPosition(Point position, bool throwOnFail) {
        std::vector<Entity *> entities;

        for (Entity * entity : Entities) {
            if (!entity->HasComponent(ComponentType::Transform)) continue;

            Components::Transform* transform = entity->GetComponent<Components::Transform>(true);

            if (position[0] > transform->Position[0] - transform->ScaleX &&
                position[0] < transform->Position[0] + transform->ScaleX &&
                position[1] > transform->Position[1] - transform->ScaleY &&
                position[1] < transform->Position[1] + transform->ScaleY)
                entities.push_back(entity);
        }

        if (throwOnFail && entities.size() == 0) {
            std::stringstream ss;
            ss << "No entities found at point " << position << ".";

            throw EntityNotFoundException(0, ss.str());
        }

        return entities;
    }

    Entity* State::GetEntityCollidingAtPoint(Point position, bool throwOnFail) {
        LevelComponents::Collision* collision = GetLevelComponent<LevelComponents::Collision>();

        if (!collision) {
            if (throwOnFail) {
                std::stringstream ss;
                ss << "No entities found at point " << position << ".";
                ss << "  This is because there is no collision level component attached.";

                throw EntityNotFoundException(0, ss.str());
            }

            return 0;
        }

        Entity* entity = collision->GetEntityCollidingAtPoint(Entities, position);

        if (!entity && throwOnFail) {
            std::stringstream ss;
            ss << "No entities found at point " << position << ".";

            throw EntityNotFoundException(0, ss.str());
        }

        return entity;
    }

    std::vector<Entity *>& State::GetAllEntities() {
        return Entities;
    }

    void State::DeleteAllLevelComponents() {
        for (LevelComponent* lc : LevelComponents) {
            delete lc;
        }

        LevelComponents.clear();
    }

    /*!
        Getter for the name of a state.

        \return
            The std::string name.
    */
    std::string State::GetName() { return Name; }

    /*!
        Setter for the name of a state.

        \param name
            The new name for the state.
    */
    void State::SetName(std::string name) { Name = name; }

    void State::Serialize(Serializer& root) {
        Serializer state = root.GetChild("State");
        state.WriteString("Name", Name);

        ArraySerializer jsonEntities(state);
        jsonEntities = state.GetChild("Entities");

        for (Entity* entity : Entities) {
            if (!(entity->IncludeInSerialize)) continue;
            Serializer entitySerializer;
            entity->Serialize(entitySerializer);
            jsonEntities.Append(entitySerializer);
        }

        ArraySerializer jsonLevelComponents(state);
        jsonLevelComponents = state.GetChild("LevelComponents");

        for (LevelComponent* lc : LevelComponents) {
            Serializer lcSerializer;
            lc->Serialize(lcSerializer);
            jsonLevelComponents.Append(lcSerializer);
        }

        state.Append(jsonLevelComponents, "LevelComponents");
        state.Append(jsonEntities, "Entities");
        root.Append(state, "State");
    }

    void State::Deserialize(Serializer& root) {
        Serializer state = root.GetChild("State");

        state.ReadString("Name", Name);
        ArraySerializer jsonEntities(state);
        jsonEntities = state.GetChild("Entities");

        for (unsigned i = 0; i < jsonEntities.Size(); ++i) {
            Serializer entitySerializer = jsonEntities[i];

            Entity* entity = new Entity();
            entity->Deserialize(entitySerializer);
            Entities.push_back(entity);
        }

        ArraySerializer jsonLevelComponents(state);
        jsonLevelComponents = state.GetChild("LevelComponents");

        for (unsigned i = 0; i < jsonLevelComponents.Size(); ++i) {
            Serializer lcSerializer = jsonLevelComponents[i];
            AddLevelComponent(DeserializeLevelComponent(lcSerializer, *this));
        }
    }

    LevelComponent* DeserializeLevelComponent(Serializer& root, State& state) {
        unsigned type;
        root.ReadUint("Type", type);

        LevelComponent* lc = 0;

        switch (static_cast<ComponentType>(type)) {
            case ComponentType::Collision:
                lc = new LevelComponents::Collision(state);
                break;
            case ComponentType::Physics:
                lc = new LevelComponents::Physics(state);
                break;
            case ComponentType::Renderer:
                lc = new LevelComponents::Renderer(state);
                break;
            default:
                return 0;
        }

        lc->Deserialize(root);

        return lc;
    }

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
