from pytest import mark
from src.py.serial_connection import SerialConnection

PAIRS_LEVEL = [
    ('level:', 'Malformed command! Missing one or more colons'),
    ('level:5', 'Malformed command! Missing one or more colons'),
    ('level:5:1000', 'Malformed command! Missing one or more colons'),
    ('level:5:1000:rising', 'Malformed command! Missing one or more colons'),
    ('level:5:1000:rising:800', 'Malformed command! Missing one or more colons'),
    ('level:5:1000:foobar:800:25', 'Invalid trigger type!'),
    ('level:5:1000::800:25', 'Could not parse trigger type!'),
    ('level:5:1000:rising::25', 'Could not parse trigger level!'),
    ('level:5:1000:rising:800:', 'Could not parse epsilon!'),
    ('level:5:1000:rising:4:25', 'Trigger level must be at least 0.025 volts!'),
    ('level:5:1000:rising:1024:25', 'Trigger level cannot exceed 5 volts!'),
    ('level:5:1000:rising:800:5', 'Epsilon must be at least 0.05 volts!'),
    ('level:5:1000:rising:800:1025', 'Epsilon cannot exceed 5 volts!')
]

@mark.parametrize('command, error', PAIRS_LEVEL)
def test_level(connection: SerialConnection, command: str, error: str) -> None:

    connection.send_message(command)
    rv, message = connection.receive_message()

    assert not rv
    assert message == error
