from src.py.serial_connection import SerialConnection

def test_265_count(connection: SerialConnection) -> None:

    connection.send_message('roll:265:1000000')

    rv, _ = connection.receive_message()  # voltage
    assert rv

    rv, _ = connection.receive_message()  # times
    assert rv
