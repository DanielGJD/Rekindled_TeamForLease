/*!
    \file   Level.cpp

    \author Sean McGeer

    \date   9/17/15

    \brief
        Implements the Level class.

    \see Level.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Level.h"
#include "Mouse.h"

namespace ForLeaseEngine {

    Level::Level(std::string file) : File(file) {
        Serializer serial;
        serial.ReadFile(File);
        serial = serial.GetChild("State");
        serial.ReadString("Name", Name);
    }

    Level::~Level() {}

    void Level::Load() {}

    void Level::Initialize() {
        Serializer serial;
        serial.ReadFile(File);
        Deserialize(serial);
        ForLease->FrameRateController().TimeScaling(1);
        Mouse::SetMouseVisible(false);
    }

    void Level::Update() {
        std::cout << "Level Update" << std::endl;
        ForLease->sound->Update();
        ForLease->OSInput.ProcessAllInput();

        for (Entity* entity : Entities) {
            entity->Update();
        }

        LevelComponents::Light* light = 0;
        LevelComponents::Renderer* renderer = 0;

        for (LevelComponent* levelComponent : LevelComponents) {
            if (levelComponent->GetType() == ComponentType::Light) {
                std::cout << "Skipping lighting" << std::endl;
                light = reinterpret_cast<LevelComponents::Light*>(levelComponent);
            }
            else if (levelComponent->GetType() == ComponentType::Renderer) {
                std::cout << "Skipping Renderer" << std::endl;
                renderer = reinterpret_cast<LevelComponents::Renderer*>(levelComponent);
            }
            else {
                std::cout << "Updating level component" << std::endl;
                levelComponent->Update(Entities);
            }
        }

        if (light) light->Update(Entities);
        if (renderer) renderer->Update(Entities);

        UpdateDebug();

        ForLease->GameWindow->UpdateGameWindow();

        if (ResetToCheckpoint) {
            LevelComponents::Checkpoint* lcCheckpoint = GetLevelComponent<LevelComponents::Checkpoint>();
            if (lcCheckpoint) {
                DeserializeNonReference(lcCheckpoint->LastCheckpointState);
            }
            ResetToCheckpoint = false;
        }

        PostFrameCleanup();
    }

    void Level::Deinitialize() {
        DeleteAllEntities();
        DeleteAllLevelComponents();
        Mouse::SetMouseVisible(true);
    }

    void Level::Unload() {}

} // ForLeaseEngine
