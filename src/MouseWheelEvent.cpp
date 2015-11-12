#include "MouseWheelEvent.h"

namespace ForLeaseEngine {
    MouseWheelEvent::MouseWheelEvent(std::string EventName) : Event(EventName),
                                     Horizontal(0), Vertical(0) {}

    MouseWheelEvent::MouseWheelEvent(std::string EventName, int horizontal, int vertical) : Event(EventName),
                                     Horizontal(horizontal), Vertical(vertical) {}

    std::ostream& operator<<(std::ostream& os, MouseWheelEvent const& rhs) {
        os << rhs.EventName << ":" << std::endl
           << "Horizontal: " << rhs.Horizontal << std::endl
           << "Vertical: " << rhs.Vertical;

        return os;
    }
}
