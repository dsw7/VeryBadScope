from src.py.serial_connection import SerialConnection

def test_220_count(connection: SerialConnection) -> None:

    connection.send_message('read:220:1000000')

    rv, _ = connection.receive_message()  # voltage
    assert rv

    rv, _ = connection.receive_message()  # times
    assert rv

def test_230_count(connection: SerialConnection) -> None:

    connection.send_message('read:230:1000000')
    rv, error = connection.receive_message()

    assert not rv
    assert 'An exception occurred when decoding results:' in error
