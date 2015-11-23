/*!
    \file   Platforms.h
    \author Sean McGeer
    \date   10/10/15
    \brief
        Provides platform checking with defines.

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef PLATFORMS_H
#define PLATFORMS_H

    // Check to see if it's 32 bit Windows
    #if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) || defined(MINGW)
    #include <Windows.h>
    #define FLE_WINDOWS
    #else
    #define FLE_NIX
    #endif

#endif
