from src.py.serial_connection import SerialConnection

def test_trigger_1(connection: SerialConnection) -> None:

    connection.send_message('trigger:')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Malformed command!'
