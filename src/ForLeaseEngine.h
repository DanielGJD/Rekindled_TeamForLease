/*!
    \file   ForLeaseEngine.h
    \author Sean McGeer
    \date   9/9/15
    \brief
        Defines the ForLeaseEngine class, which is itself an instance of the For Lease Engine.
*/

#ifndef FLE_H
#define FLE_H

/*!
    \class ForLeaseEngine
    \brief
        The base of the game engine.
*/
class ForLeaseEngine {

    public:
        ForLeaseEngine();
        ForLeaseEngine(int resolutionX, int resolutionY, int frameRate);

    private:
        //! Width of the screen
        int resolutionX;
        //! Height of the screen
        int resolutionY;
        //! Frame rate of the game
        int frameRate;

};

#endif