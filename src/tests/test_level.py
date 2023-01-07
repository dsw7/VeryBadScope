from pytest import mark
from src.py.serial_connection import SerialConnection

PAIRS_LEVEL = [
    ('level:', 'Malformed command! Missing one or more colons'),
    ('level:5', 'Malformed command! Missing one or more colons'),
    ('level:5:1000', 'Malformed command! Missing one or more colons'),
    ('level:5:1000:rising', 'Malformed command! Missing one or more colons'),
    ('level:5:1000:foobar:4', 'Invalid trigger type!'),
    ('level:5:1000::4', 'Could not parse trigger type!'),
    ('level:5:1000:rising:', 'Could not parse trigger level!'),
    ('level:5:1000:rising:4', 'Trigger level must be at least 0.025 volts!'),
    ('level:5:1000:rising:1024', 'Trigger level cannot exceed 5 volts!')
]

@mark.parametrize('command, error', PAIRS_LEVEL)
def test_level(connection: SerialConnection, command: str, error: str) -> None:

    connection.send_message(command)
    rv, message = connection.receive_message()

    assert not rv
    assert message == error
