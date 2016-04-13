/*!
    \file   WindowEvent.h
    \author Christopher Hudson

    \brief
        Defines a class containing info on window changes

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Event.h"

namespace ForLeaseEngine {
    class WindowEvent : public Event {
        public:
            static const std::string Shown;
            static const std::string Hidden;
            static const std::string Exposed;
            static const std::string Moved;
            static const std::string Resized;
            static const std::string SizeChanged;
            static const std::string Minimized;
            static const std::string Maximized;
            static const std::string Restored;
            static const std::string MouseEnter;
            static const std::string MouseLeave;
            static const std::string FocusGained;
            static const std::string FocusLost;
            static const std::string Close;

            int X;
            int Y;

            WindowEvent(std::string eventName, int x = 0, int y = 0);
            friend std::ostream& operator<<(std::ostream& os, WindowEvent& rhs);
    };
}
