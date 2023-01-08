#pragma once

#include "command_base.h"

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
