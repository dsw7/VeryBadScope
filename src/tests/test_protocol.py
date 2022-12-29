from src.py.serial_connection import SerialConnection

def test_hello(connection: SerialConnection) -> None:

    connection.send_message('hello')
    rv, message = connection.receive_message()

    assert rv
    assert message == 'Hello from InoDAQ2. I should blink 5 times!'
