import sys
from typing import Dict, Union
from click import secho
from serial_connection import SerialConnection

def command_hello(**connection_params: Dict[str, Union[bool, str]]) -> None:

    secho('> Handshaking with device', fg='yellow')
    rv = False

    with SerialConnection(**connection_params) as connection:
        connection.send_message(b'hello')
        rv, error = connection.receive_message()

    if not rv:
        sys.exit(f'Command failed with error: "{error}"')
