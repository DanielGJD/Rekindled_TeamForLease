/*!
    \file   DigiPenLogo.h
    \author Sean McGeer
    \date   03/03/16
    \brief
        Defines the DigiPen logo screen state.
    \see State.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef DIGIPEN_LOGO_H
#define DIGIPEN_LOGO_H

#include "Engine.h"
#include "Entity.h"
#include "Event.h"
#include "LevelComponentsInclude.h"
#include "State.h"

class DigiPenLogo : public ForLeaseEngine::State {
public:
    DigiPenLogo();
    void Load();
    void Initialize();
    void Update();
    void Deinitialize();
    void Unload();
    
    void OnMouseButtonEvent(const ForLeaseEngine::Event* e);
private:
    enum FadeState : unsigned {
        FadingIn,
        FadingOut,
        Hold
    };
    float FadeInTime = 5.0f;
    float HoldTime = 2.0f;
    float FadeOutTime = 1.0f;
    FadeState CurrentFadeState;
};

#endif // DIGIPEN_LOGO_H
