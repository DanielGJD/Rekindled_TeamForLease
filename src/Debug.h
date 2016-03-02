/*!
    \author Sean McGeer
    
    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef DEBUG_H
#define DEBUG_H

namespace ForLeaseEngine
{
    namespace Modules {
        class Debug {
            public:
                Debug();

                struct Draw {
                    bool Collision = false;
                };

                Draw Draw;
        };
    }
}

#endif
