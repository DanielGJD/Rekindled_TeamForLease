/*!
    \file   LevelEditor.h
    \author Sam Montanari
    \date   12/2/15

    \brief
        Definition of level editor class

    \see LevelEditor.cpp

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include "LevelComponentsInclude.h"
#include "Mesh.h"
#include "Vector.h"
#include "Engine.h"
#include "Event.h"
#include "Entity.h"
#include "ComponentsInclude.h"
#include "ComponentFinaleOne.h"

namespace ForLeaseEngine
{
    class LevelEditor : public State
    {
    public:
        void Load();
        void Initialize();
        void Update();
        void Deinitialize();
        void Unload();
        void LoadFiles();
        void Input();

    private:
        void DrawMainMenu();
        void SaveLevel(std::string level);
        void SpawnBluePrint(std::string blueprint);
        void LoadLevel(std::string level);
        std::string RequiredToolTip(std::string component);
        void DrawMainWindow();
        void DrawObjectWindow();
        void MakeSelection();
        void GetMouse(Point& v);
        void AddComponent(const std::string& component);
    };
}



#endif // LEVELEDITOR_H
