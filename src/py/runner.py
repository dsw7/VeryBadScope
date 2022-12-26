from typing import Dict, Union
import click
from serial_connection import SerialConnection

@click.group()
@click.option('-p', '--port', default='/dev/ttyS2', help='Specify USB port', metavar='<com-port>')
@click.option('-d', '--debug', is_flag=True, help='Enable debug logging')
@click.pass_context
def main(context: click.core.Context, **configs: Dict[str, Union[bool, str]]) -> None:
    context.obj = configs

@main.command(help='Handshake with device')
@click.pass_obj
def hello(obj: Dict[str, Union[bool, str]]) -> None:

    with SerialConnection(**obj) as connection:
        connection.send_message(b'hello')
        connection.receive_message()

@main.command(help='Collect a trace')
@click.option('-n', '--count', default=5, help='Specify number of reads', metavar='<num-reads>')
@click.pass_obj
def read(obj: Dict[str, Union[bool, str]], count: str) -> None:

    with SerialConnection(**obj) as connection:
        connection.send_message(f'read:{count}'.encode())
        connection.receive_message()

if __name__ == '__main__':
    main()
