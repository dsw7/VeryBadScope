from src.py.serial_connection import SerialConnection

def test_trigger_1(connection: SerialConnection) -> None:

    connection.send_message('trigger:')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Malformed command! Missing one or more colons'

def test_trigger_2(connection: SerialConnection) -> None:

    connection.send_message('trigger:5')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Malformed command! Missing one or more colons'

def test_trigger_3(connection: SerialConnection) -> None:

    connection.send_message('trigger:5:1000')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Malformed command! Missing one or more colons'

def test_trigger_4(connection: SerialConnection) -> None:

    connection.send_message('trigger:5:1000:rising')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Malformed command! Missing one or more colons'

def test_trigger_5(connection: SerialConnection) -> None:

    connection.send_message('trigger:5:1000:foobar:4')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Invalid trigger type!'

def test_trigger_6(connection: SerialConnection) -> None:

    connection.send_message('trigger:5:1000::4')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Could not parse trigger type!'
