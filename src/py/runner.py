from typing import Dict
import click
from serial_connection import SerialConnection

@click.group()
def main() -> None:
    pass

@main.command()
@click.option('-p', '--port')
def hello(**configs: Dict[str, str]) -> None:

    with SerialConnection(serial_port=configs['port']) as connection:
        connection.send_message(b'hello')
        connection.receive_message()

@main.command()
@click.option('-p', '--port')
@click.option('-n', '--count')
def read(**configs: Dict[str, str]) -> None:

    with SerialConnection(serial_port=configs['port']) as connection:
        connection.send_message(b'hello')
        connection.receive_message()

        command = f"read:{configs['count']}".encode()
        connection.send_message(command)
        connection.receive_message()

if __name__ == '__main__':
    main()
