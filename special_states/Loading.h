/*!
    \file   Loading.h
    \author Sean McGeer
    \date   2/15/16
    \brief
        Defines the loading screen state.
    \see State.h

    \copyright ęCopyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef LOADING_H
#define LOADING_H

#include "Engine.h"
#include "Entity.h"
#include "Event.h"
#include "LevelComponentsInclude.h"
#include "State.h"
#include <string>

class Loading : public ForLeaseEngine::State {
public:
    Loading(std::string loadfile);
    void Load();
    void Initialize();
    void Update();
    void Deinitialize();
    void Unload();

private:
    std::string LoadFile;
};

#endif // LOADING_H