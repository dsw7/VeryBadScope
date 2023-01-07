from typing import Dict, Union
import click
from command_hello import command_hello
from command_roll import CommandRoll
from command_edge import CommandEdgeTrigger
from command_level import CommandLevelTrigger

@click.group()
@click.option('-p', '--port', default='/dev/ttyS2', help='Specify USB port', metavar='<com-port>')
@click.option('-d', '--debug', is_flag=True, help='Enable debug logging')
@click.pass_context
def main(context: click.core.Context, **configs: Dict[str, Union[bool, str]]) -> None:
    context.obj = configs

@main.command(help='Handshake with device')
@click.pass_obj
def hello(obj: Dict[str, Union[bool, str]]) -> None:
    command_hello(**obj)

@main.command(help='Collect a trace without any triggering')
@click.option('-n', '--record-length', default=5, help='Specify number of reads', metavar='<num-reads>')
@click.option('-r', '--measurement-duration', default=1000, help='Specify time range to read over', metavar='<microseconds>')
@click.option('-p', '--plot', is_flag=True, help='Plot results')
@click.option('-q', '--plot-path', default='/tmp/roll.png', help='Where to dump plot if --plot is specified', metavar='<path>')
@click.option('-t', '--to-csv', is_flag=True, help='Export results to CSV')
@click.option('-u', '--csv-path', default='/tmp/roll.csv', help='Where to dump CSV if --to-csv is specified', metavar='<path>')
@click.pass_obj
def roll(obj: Dict[str, Union[bool, str]], **options: Dict[str, str]) -> None:
    CommandRoll(**obj, **options).main()

@main.command(help='Collect a trace with rising- or falling edge triggering')
@click.option('-n', '--record-length', default=5, help='Specify number of reads after the trigger', metavar='<num-reads>')
@click.option('-r', '--measurement-duration', default=1000, help='Specify time range to read over', metavar='<microseconds>')
@click.option('-t', '--trigger', type=click.Choice(['rising', 'falling']), default='rising', help='Specify trigger type')
@click.option('-d', '--delta', default=4, help='Specify voltage difference that results in trigger', metavar='<voltage>', type=float)
@click.option('-p', '--plot', is_flag=True, help='Plot results')
@click.option('-q', '--plot-path', default='/tmp/edge.png', help='Where to dump plot if --plot is specified', metavar='<path>')
@click.option('-t', '--to-csv', is_flag=True, help='Export results to CSV')
@click.option('-u', '--csv-path', default='/tmp/edge.csv', help='Where to dump CSV if --to-csv is specified', metavar='<path>')
@click.pass_obj
def edge(obj: Dict[str, Union[bool, str]], **options: Dict[str, str]) -> None:
    CommandEdgeTrigger(**obj, **options).main()

@main.command(help='Collect a trace with rising- or falling edge triggering at a specific level')
@click.option('-n', '--record-length', default=5, help='Specify number of reads after the trigger', metavar='<num-reads>')
@click.option('-r', '--measurement-duration', default=1000, help='Specify time range to read over', metavar='<microseconds>')
@click.option('-t', '--trigger', type=click.Choice(['rising', 'falling']), default='rising', help='Specify trigger type')
@click.option('-l', '--level', default=4, help='Specify level at which to trigger', metavar='<voltage>', type=float)
@click.option('-p', '--plot', is_flag=True, help='Plot results')
@click.option('-q', '--plot-path', default='/tmp/level.png', help='Where to dump plot if --plot is specified', metavar='<path>')
@click.option('-t', '--to-csv', is_flag=True, help='Export results to CSV')
@click.option('-u', '--csv-path', default='/tmp/level.csv', help='Where to dump CSV if --to-csv is specified', metavar='<path>')
@click.pass_obj
def level(obj: Dict[str, Union[bool, str]], **options: Dict[str, str]) -> None:
    CommandLevelTrigger(**obj, **options).main()

if __name__ == '__main__':
    main()
