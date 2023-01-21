from pytest import fixture
from src.py.serial_connection import SerialConnection

@fixture(scope='session')
def connection() -> SerialConnection:
    with SerialConnection(port='/dev/ttyS2', debug=False) as conn:
        yield conn
