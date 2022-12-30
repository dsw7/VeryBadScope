from time import perf_counter_ns
from typing import Dict, Union
import sys
from click import secho
from serial_connection import SerialConnection
import helpers

def command_hello(**cli_params: Dict[str, Union[bool, str]]) -> None:

    secho('> Handshaking with device', fg='yellow')
    rv = False

    with SerialConnection(**cli_params) as connection:
        connection.send_message('hello')
        rv, error = connection.receive_message()

        if not rv:
            sys.exit(error)

def command_read(**cli_params: Dict[str, Union[bool, str]]) -> None:

    secho('> Reading data from device', fg='yellow')

    count = cli_params['count']
    time_range = cli_params['time_range']

    result_v = None
    result_t = None

    with SerialConnection(**cli_params) as connection:

        start = perf_counter_ns()
        connection.send_message(f'read:{count}:{time_range}')

        rv, result_v = connection.receive_message() # voltages
        if not rv:
            sys.exit(result_v)

        rv, result_t = connection.receive_message() # times
        if not rv:
            sys.exit(result_t)

        end = (perf_counter_ns() - start) / 1_000_000
        secho(f'> Round trip time: {end} ms', fg='yellow')

    times = helpers.results_to_ints(result_t)
    voltages = helpers.results_to_ints(result_v)

    times_n = helpers.normalize_time_series(times)
    voltages_n = helpers.normalize_voltage_series(voltages)

    if cli_params['plot']:
        pass
    else:
        helpers.peek(times_n, voltages_n)
