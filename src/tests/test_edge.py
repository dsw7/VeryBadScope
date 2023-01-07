from pytest import mark
from src.py.serial_connection import SerialConnection

PAIRS_EDGE = [
    ('edge:', 'Malformed command! Missing one or more colons'),
    ('edge:5', 'Malformed command! Missing one or more colons'),
    ('edge:5:1000', 'Malformed command! Missing one or more colons'),
    ('edge:5:1000:rising', 'Malformed command! Missing one or more colons'),
    ('edge:5:1000:foobar:4', 'Invalid trigger type!'),
    ('edge:5:1000::4', 'Could not parse trigger type!'),
    ('edge:5:1000:rising:', 'Could not parse trigger delta!'),
    ('edge:5:1000:rising:19', 'Trigger delta must be at least 0.1 volts!'),
    ('edge:5:1000:rising:1024', 'Trigger delta cannot exceed 5 volts!')
]

@mark.parametrize('command, error', PAIRS_EDGE)
def test_edge(connection: SerialConnection, command: str, error: str) -> None:

    connection.send_message(command)
    rv, message = connection.receive_message()

    assert not rv
    assert message == error
