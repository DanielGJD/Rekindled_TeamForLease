/*!
    \file   MainMenu.h
    \author Sean McGeer
    \date   11/22/15
    \brief
        Defines the main menu screen state.
    \see State.h

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Engine.h"
#include "Entity.h"
#include "Event.h"
#include "LevelComponentsInclude.h"
#include "State.h"
#include "Timer.h"

class MainMenu : public ForLeaseEngine::State {
public:
    MainMenu();
    void Load();
    void Initialize();
    void Update();
    void Deinitialize();
    void Unload();
    std::string MainMenuBGM;
private:
    int Health;
    ForLeaseEngine::Timer AniTimer;
};

#endif // MAIN_MENU_H
