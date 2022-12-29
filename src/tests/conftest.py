from pytest import fixture
from src.py.serial_connection import SerialConnection

@fixture(scope='module')
def connection() -> SerialConnection:

    connection_params = {
        "baudrate": 19200,
        "parity": "N",
        "stopbits": 1,
        "bytesize": 8,
        "timeout": 5,
        "port": "/dev/ttyS2",
        "debug": False
    }

    with SerialConnection(**connection_params) as connection:
        yield connection
