import sys
import logging
from time import sleep
from json import dumps
from typing import TypeVar
import serial

BAUD_RATE = 9600
T = TypeVar('T')


class SerialConnection:

    def __init__(self: T, port: str, debug: bool) -> T:

        self.serial_port_obj = None
        self.port = port

        if debug:
            level = logging.DEBUG
        else:
            level = logging.INFO

        configs = {
            'level': level,
            'format': '%(asctime)s.%(msecs)03d %(message)s',
            'datefmt': '%Y-%m-%d %I:%M:%S'
        }

        logging.basicConfig(**configs)
        self.logger = logging.getLogger(__class__.__name__)

    def __enter__(self: T) -> T:

        connection_params = {
            'baudrate': BAUD_RATE,
            'parity': serial.PARITY_NONE,
            'stopbits': serial.STOPBITS_ONE,
            'bytesize': serial.EIGHTBITS,
            'timeout': 5,
            'port': self.port
        }

        self.logger.debug('Connecting using parameters: %s', dumps(connection_params, indent=4))

        try:
            self.serial_port_obj = serial.Serial(**connection_params)
        except serial.serialutil.SerialException as exception:
            sys.exit(f'An exception occurred when connecting: "{exception}"')

        if not self.serial_port_obj.is_open:
            sys.exit(f'Could not connect to "{self.serial_port_obj.name}"')

        # Opening a connection will send a DTR (Data Terminal Ready) signal to device, which will
        # force the device to reset. Give device 2 seconds to reset

        self.logger.debug('DTR (Data Terminal Ready) was sent. Waiting for device to reset')
        sleep(2)

        return self

    def __exit__(self: T, *args) -> None:

        if self.serial_port_obj is None:
            self.logger.debug('Not closing connection. Connection was never opened!')
            return

        self.logger.debug('Closing connection!')

        if self.serial_port_obj.is_open:

            self.send_message(b'exit')
            self.receive_message()
            self.serial_port_obj.close()

    def send_message(self: T, message: bytes) -> None:

        self.logger.debug('Sending message: "%s"', message)
        self.logger.debug('Sent %i bytes', self.serial_port_obj.write(message))

    def receive_message(self: T) -> bytes:

        self.logger.debug('Waiting to receive message...')
        message_received = False

        while not message_received:

            while self.serial_port_obj.in_waiting < 1:
                pass

            bytes_from_dev = self.serial_port_obj.read_until()  # Reads until \n by default
            message_received = True

        self.logger.debug('Received message: %s', bytes_from_dev)
        return bytes_from_dev
