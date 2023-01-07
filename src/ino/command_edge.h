#pragma once

#include "command_scope_base.h"

#include "Arduino.h"

namespace Command
{
    class Edge: public ScopeBase
    {
        public:
            void acquire_data();

        private:
            int idx_trigger = 0;
            int idx_trigger_delta = 0;

            ::String trigger_type = "rising";
            int trigger_delta = 0;

            bool parse_trigger_specific_indices();
            bool parse_trigger();
            bool parse_trigger_delta();
            void trigger();
    };
}
