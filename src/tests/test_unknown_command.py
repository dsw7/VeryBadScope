from src.py.serial_connection import SerialConnection

def test_unknown_command(connection: SerialConnection) -> None:

    connection.send_message('foobar')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Unknown command: foobar'
