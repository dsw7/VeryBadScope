from time import perf_counter_ns
import sys
from click import secho
from serial_connection import SerialConnection
from command_base import CommandBase, T, volt_to_analog


class CommandEdgeTrigger(CommandBase):

    def acquire_data(self: T) -> None:

        secho('> Waiting on trigger...', fg='yellow')

        record_length = self.cli_params['record_length']
        measurement_duration = self.cli_params['measurement_duration']
        trigger = self.cli_params['trigger']
        delta = volt_to_analog(self.cli_params['delta'])

        with SerialConnection(**self.cli_params) as connection:

            start = perf_counter_ns()
            connection.send_message(f'trigger:{record_length}:{measurement_duration}:{trigger}:{delta}')

            rv, self.raw_v = connection.receive_message()
            if not rv:
                sys.exit(self.raw_v)

            rv, self.raw_t = connection.receive_message()
            if not rv:
                sys.exit(self.raw_t)

            end = (perf_counter_ns() - start) / 1_000_000
            secho(f'> Round trip time: {end} ms', fg='yellow')
