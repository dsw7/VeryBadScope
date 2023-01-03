#pragma once

#include "command_scope_base.h"

namespace Command
{
    class Trigger: public ScopeBase
    {
        public:
            void acquire_data();

        private:
            void trigger();
    };
}
