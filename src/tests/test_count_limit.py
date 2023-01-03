from src.py.serial_connection import SerialConnection

def test_265_count(connection: SerialConnection) -> None:

    connection.send_message('roll:265:1000000')

    rv, _ = connection.receive_message()  # voltage
    assert rv

    rv, _ = connection.receive_message()  # times
    assert rv

def test_280_count(connection: SerialConnection) -> None:

    connection.send_message('roll:280:1000000')
    rv, error = connection.receive_message()

    assert not rv
    assert 'An exception occurred when decoding results:' in error
