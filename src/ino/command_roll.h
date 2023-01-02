#pragma once

#include "command_scope_base.h"

namespace Command
{
    class Roll: public ScopeBase
    {
        public:
            void acquire_data();

        private:
            void roll();
    };
}
