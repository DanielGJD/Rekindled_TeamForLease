
#include "Level.h"

namespace ForLeaseEngine {

    Level::Level(std::string file) : File(file) {}

    Level::~Level() {}

    void Level::Load() {}

    void Level::Initialize() {
        Serializer serial;
        serial.ReadFile(File);
        Deserialize(serial);
    }

    void Level::Update() {
        ForLease->OSInput.ProcessAllInput();

        for (Entity* entity : Entities) {
            entity->Update();
        }

        for (LevelComponent* levelComponent : LevelComponents) {
            levelComponent->Update(Entities);
        }

        ForLease->GameWindow->UpdateGameWindow();
    }

    void Level::Deinitialize() {
        DeleteAllEntities();
        DeleteAllLevelComponents();
    }

    void Level::Unload() {}

}