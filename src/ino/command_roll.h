#pragma once

#include "Arduino.h"

namespace Command
{
    class ScopeBase
    {
        public:
            void set_command(const ::String &command);
            virtual void read();

        protected:
            bool validate_command();
            bool parse_command_indices();
            bool parse_record_length();
            bool parse_measurement_duration();
            bool compute_period();

            ::String command = "read:5:1000";

            int idx_record_length = 0;
            int idx_measurement_duration = 0;

            long record_length = 0;
            long measurement_duration = 0;
            long period = 0;
    };

    void roll(const String &command);
}
