/*!
    \file   State.h
    \author Sean McGeer
    \date   9/17/15
    \brief
        Defines the State class.
    \see State.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
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
            Entity* AddEntity(std::string name = "");
            Entity* SpawnArchetype(std::string filename, Point position = Point(), std::string name = "");
            bool DeleteEntity(long unsigned id);
            bool DeleteEntity(std::string name);
            bool DeleteEntity(Entity* entity);
            void DeleteAllEntities();
            Entity* GetEntityByID(long unsigned id, bool throwOnFail = false);
            Entity* GetEntityByName(std::string name, bool throwOnFail = false);
            Entity* GetEntityAtPosition(Point position, bool throwOnFail = false);
            std::vector<Entity *> GetEntitiesAtPosition(Point position, bool throwOnFail = false);
            Entity* GetEntityCollidingAtPoint(Point position, bool throwOnFail = false);
            //std::vector<Entity*> GetEntitiesInRadius(Point const& position, float radius);
            //std::vector<Entity*> GetEntitiesInCone(Point const& position, float radius, Vector const& direction, float angle);
            std::vector<Entity *>& GetAllEntities();

            void DeleteAllLevelComponents();


            /*!
                Get a LevelComponent from this State.

                \param throwOnFail
                    A boolean determining whether to throw an error on failure, rather
                    than the default behavior, which is to return a null pointer.
            */
            template <typename T = LevelComponent>
            T* GetLevelComponent(bool throwOnFail = false) const {
                if (T::Type == ComponentType::None && throwOnFail)
                    return 0;

                for (LevelComponent* lc : LevelComponents)
                    if (lc->GetType() == T::Type)
                        return reinterpret_cast<T*>(lc);

                return 0;
            }

            std::string GetName();
            void SetName(std::string name);

            virtual void Serialize(Serializer& root);
            virtual void Deserialize(Serializer& root);

        protected:
            std::string Name;                               //! Name of the state
            std::vector<LevelComponent*> LevelComponents;   //! The level components in the state
            std::vector<Entity*> Entities;                  //! The entities in the state

    };

    LevelComponent* DeserializeLevelComponent(Serializer& root, State& state);
    void AddLevelComponentsToState(ComponentType mask, State* state);

} // ForLeaseEngine

#endif
