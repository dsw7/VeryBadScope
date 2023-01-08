#pragma once

#include "Arduino.h"

namespace Command
{
    class ScopeBase
    {
        public:
            void set_command(const ::String &command);
            virtual void acquire_data() = 0;

        protected:
            unsigned int read_pin = A0;
            ::String command = "roll:5:1000";

            int idx_record_length = 0;
            int idx_measurement_duration = 0;

            long record_length = 0;
            long measurement_duration = 0;
            long corrected_period = 0;

            bool parse_command_indices();
            bool parse_record_length();
            bool parse_measurement_duration();
            bool compute_period();
    };
}
