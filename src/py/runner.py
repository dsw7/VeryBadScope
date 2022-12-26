from typing import Dict, Union
from time import perf_counter_ns
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
    click.secho('> Handshaking with device', fg='yellow')

    with SerialConnection(**obj) as connection:
        connection.send_message(b'hello')
        connection.receive_message()

@main.command(help='Collect a trace')
@click.option('-n', '--count', default=5, help='Specify number of reads', metavar='<num-reads>')
@click.pass_obj
def read(obj: Dict[str, Union[bool, str]], count: str) -> None:
    click.secho('> Reading data from device', fg='yellow')

    with SerialConnection(**obj) as connection:
        start = perf_counter_ns()
        connection.send_message(f'read:{count}'.encode())
        connection.receive_message()

        end = (perf_counter_ns() - start) / 1_000_000
        click.secho(f'> Round trip time: {end} ms', fg='yellow')

if __name__ == '__main__':
    main()
