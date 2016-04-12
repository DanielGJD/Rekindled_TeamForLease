/*!
    \file   Level.cpp

    \author Sean McGeer

    \date   9/17/15

    \brief
        Implements the Level class.

    \see Level.h

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
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
        ForLease->sound->Update(ForLease->FrameRateController().GetDt());
        ForLease->OSInput.ProcessAllInput();

        for (Entity* entity : Entities) {
            entity->Update();
        }

        for (LevelComponent* levelComponent : LevelComponents) {
            levelComponent->Update(Entities);
        }

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
