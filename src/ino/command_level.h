#pragma once

#include "command_base.h"

#include "Arduino.h"

namespace Command
{
    class Level: public ScopeBase
    {
        public:
            void acquire_data();

        private:
            int idx_trigger = 0;
            int idx_trigger_level = 0;
            int idx_epsilon = 0;

            ::String trigger_type = "rising";
            int trigger_level = 0;
            int epsilon = 0;

            bool parse_level_command_specific_indices();
            bool parse_trigger();
            bool parse_trigger_level();
            bool parse_epsilon();
            void wait_for_trigger();
            void start_reading_after_trigger();
    };
}
