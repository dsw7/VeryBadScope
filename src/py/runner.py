from typing import Dict
import click
from serial_connection import SerialConnection

@click.group()
@click.option('-p', '--port', default='/dev/ttyS2', help='Specify USB port', metavar='<com-port>')
@click.pass_context
def main(context: click.core.Context, port: str) -> None:
    context.obj = {
        'port': port
    }

@main.command(help='Handshake with device')
@click.pass_obj
def hello(obj: Dict[str, str]) -> None:

    with SerialConnection(serial_port=obj['port']) as connection:
        connection.send_message(b'hello')
        connection.receive_message()

@main.command(help='Collect a trace')
@click.option('-n', '--count', default=5, help='Specify number of reads', metavar='<num-reads>')
@click.pass_obj
def read(obj: Dict[str, str], count: str) -> None:

    with SerialConnection(serial_port=obj['port']) as connection:
        connection.send_message(f'read:{count}'.encode())
        connection.receive_message()

if __name__ == '__main__':
    main()
