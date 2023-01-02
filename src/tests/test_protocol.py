from src.py.serial_connection import SerialConnection

def test_unknown_command(connection: SerialConnection) -> None:

    connection.send_message('foobar')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Unknown command: foobar'

def test_hello_1(connection: SerialConnection) -> None:

    connection.send_message('hello')
    rv, message = connection.receive_message()

    assert rv
    assert message == 'Hello from InoDAQ2. I should blink 5 times!'

def test_hello_2(connection: SerialConnection) -> None:

    connection.send_message('hello:')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Unknown command: hello:'

def test_read_1(connection: SerialConnection) -> None:

    connection.send_message('read:')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Malformed read command!'

def test_read_2(connection: SerialConnection) -> None:

    connection.send_message('read:5:1000')

    rv, _ = connection.receive_message()  # voltage
    assert rv

    rv, _ = connection.receive_message()  # times
    assert rv

def test_read_3(connection: SerialConnection) -> None:

    connection.send_message('read::1000')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Could not parse record length!'

def test_read_4(connection: SerialConnection) -> None:

    connection.send_message('read:abc:1000')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Could not parse record length!'

def test_read_5(connection: SerialConnection) -> None:

    connection.send_message('read:-1:1000')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Record length must be at least 5 reads!'

def test_read_6(connection: SerialConnection) -> None:

    connection.send_message('read:5:')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Could not parse measurement duration!'

def test_read_7(connection: SerialConnection) -> None:

    connection.send_message('read:5:abc')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Could not parse measurement duration!'

def test_read_8(connection: SerialConnection) -> None:

    connection.send_message('read:5:-1')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Minimum measurement duration is 1000 microseconds!'

def test_read_9(connection: SerialConnection) -> None:

    connection.send_message('read:10:1000')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Computed period is too short. Try a greater range to count ratio!'

def test_read_10(connection: SerialConnection) -> None:

    connection.send_message('read:5:2000000')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Computed period is too long. Try a lesser range to count ratio!'
