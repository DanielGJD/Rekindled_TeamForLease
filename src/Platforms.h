/*!
    \file   Platforms.h
    \author Sean McGeer
    \date   10/10/15
    \brief
        Provides platform checking with defines.
*/

#ifndef PLATFORMS_H
#define PLATFORMS_H

    // Check to see if it's 32 bit Windows
    #if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) || defined(MINGW)
    #define FLE_WINDOWS
    #else
    #define FLE_NIX
    #endif

#endif
