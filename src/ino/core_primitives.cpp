#include "core_primitives.h"

namespace Primitives
{

void info(const String &message)
{
    ::Serial.println("1;" + message);
    ::Serial.flush();
}

void error(const String &message)
{
    ::Serial.println("0;" + message);
    ::Serial.flush();
}

} // namespace Primitives
