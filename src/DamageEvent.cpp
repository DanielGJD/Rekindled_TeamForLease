#include "DamageEvent.h"

namespace ForLeaseEngine {
    DamageEvent::DamageEvent(float damage, bool kill) : Event("DamageEvent"), Damage(damage), Kill(kill) {}
}
