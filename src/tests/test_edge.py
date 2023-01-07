from src.py.serial_connection import SerialConnection

def test_edge_1(connection: SerialConnection) -> None:

    connection.send_message('edge:')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Malformed command! Missing one or more colons'

def test_edge_2(connection: SerialConnection) -> None:

    connection.send_message('edge:5')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Malformed command! Missing one or more colons'

def test_edge_3(connection: SerialConnection) -> None:

    connection.send_message('edge:5:1000')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Malformed command! Missing one or more colons'

def test_edge_4(connection: SerialConnection) -> None:

    connection.send_message('edge:5:1000:rising')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Malformed command! Missing one or more colons'

def test_edge_5(connection: SerialConnection) -> None:

    connection.send_message('edge:5:1000:foobar:4')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Invalid trigger type!'

def test_edge_6(connection: SerialConnection) -> None:

    connection.send_message('edge:5:1000::4')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Could not parse trigger type!'

def test_edge_7(connection: SerialConnection) -> None:

    connection.send_message('edge:5:1000:rising:')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Could not parse trigger delta!'

def test_edge_8(connection: SerialConnection) -> None:

    connection.send_message('edge:5:1000:rising:19')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Trigger delta must be at least 0.1 volts!'

def test_edge_9(connection: SerialConnection) -> None:

    connection.send_message('edge:5:1000:rising:1024')
    rv, message = connection.receive_message()

    assert not rv
    assert message == 'Trigger delta cannot exceed 5 volts!'
