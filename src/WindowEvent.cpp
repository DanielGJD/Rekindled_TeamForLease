#include "WindowEvent.h"

namespace ForLeaseEngine {
    const std::string WindowEvent::Shown = "Shown";
    const std::string WindowEvent::Hidden = "Hidden";
    const std::string WindowEvent::Exposed = "Exposed";
    const std::string WindowEvent::Moved = "Moved";
    const std::string WindowEvent::Resized = "Resized";
    const std::string WindowEvent::SizeChanged = "SizeChanged";
    const std::string WindowEvent::Minimized = "Minimized";
    const std::string WindowEvent::Restored = "Restored";
    const std::string WindowEvent::MouseEnter = "MouseEnter";
    const std::string WindowEvent::MouseLeave = "MouseLeave";
    const std::string WindowEvent::FocusGained = "FocusGained";
    const std::string WindowEvent::FocusLost = "FocusLost";
    const std::string WindowEvent::Close = "Close";

    WindowEvent::WindowEvent(std::string eventName, int x, int y)
                            : Event(eventName), X(x), Y(y) {}

    std::ostream& operator<<(std::ostream& os, WindowEvent& rhs) {
        //os << EventName << " Event:" << std::endl
           //<< "  X: " << X << std::end
           //<< "  Y: " << Y << std::endl;

        return os;
    }
}
