from pytest import mark
from src.py.serial_connection import SerialConnection

def test_roll_265_count(connection: SerialConnection) -> None:

    connection.send_message('roll:265:1000000')

    rv, _ = connection.receive_message()  # voltage
    assert rv

    rv, _ = connection.receive_message()  # times
    assert rv

PAIRS_ROLL = [
    ('roll:', 'Malformed command! Missing one or more colons'),
    ('roll::1000', 'Could not parse record length!'),
    ('roll:abc:1000', 'Could not parse record length!'),
    ('roll:-1:1000', 'Record length must be at least 5 reads!'),
    ('roll:5:', 'Could not parse measurement duration!'),
    ('roll:5:abc', 'Could not parse measurement duration!'),
    ('roll:5:-1', 'Minimum measurement duration is 1000 microseconds!'),
    ('roll:10:1000', 'Computed period is too short. Try a greater range to count ratio!'),
    ('roll:5:2000000', 'Computed period is too long. Try a lesser range to count ratio!')
]

@mark.parametrize('command, error', PAIRS_ROLL)
def test_roll_errors(connection: SerialConnection, command: str, error: str) -> None:

    connection.send_message(command)
    rv, message = connection.receive_message()

    assert not rv
    assert message == error
