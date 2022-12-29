from time import perf_counter_ns
from typing import Dict, Union
import sys
from click import secho
from serial_connection import SerialConnection

def command_hello(**cli_params: Dict[str, Union[bool, str]]) -> None:

    secho('> Handshaking with device', fg='yellow')
    rv = False

    with SerialConnection(**cli_params) as connection:
        connection.send_message('hello')
        rv, error = connection.receive_message()

    if not rv:
        sys.exit(f'Command failed with error: "{error}"')

def command_read(**cli_params: Dict[str, Union[bool, str]]) -> None:

    secho('> Reading data from device', fg='yellow')

    count = cli_params['count']
    time_range = cli_params['time_range']

    with SerialConnection(**cli_params) as connection:
        start = perf_counter_ns()
        connection.send_message(f'read:{count}:{time_range}')
        connection.receive_message() # voltages
        connection.receive_message() # times

        end = (perf_counter_ns() - start) / 1_000_000
        secho(f'> Round trip time: {end} ms', fg='yellow')
