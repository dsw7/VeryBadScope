from typing import Dict, Union
import sys
from click import secho
from serial_connection import SerialConnection

def command_hello(**cli_params: Dict[str, Union[bool, str]]) -> None:
    secho('> Handshaking with device', fg='yellow')

    with SerialConnection(**cli_params) as connection:
        connection.send_message('hello')
        rv, message = connection.receive_message()

        if not rv:
            sys.exit(message)

        secho(f'> Received message from device: "{message}"', fg='yellow')
