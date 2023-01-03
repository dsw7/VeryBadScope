from typing import Dict, Union
import click
import commands

@click.group()
@click.option('-p', '--port', default='/dev/ttyS2', help='Specify USB port', metavar='<com-port>')
@click.option('-d', '--debug', is_flag=True, help='Enable debug logging')
@click.pass_context
def main(context: click.core.Context, **configs: Dict[str, Union[bool, str]]) -> None:
    context.obj = configs

@main.command(help='Handshake with device')
@click.pass_obj
def hello(obj: Dict[str, Union[bool, str]]) -> None:
    commands.command_hello(**obj)

@main.command(help='Collect a trace without any triggering')
@click.option('-n', '--record-length', default=5, help='Specify number of reads', metavar='<num-reads>')
@click.option('-r', '--measurement-duration', default=1000, help='Specify time range to read over', metavar='<microseconds>')
@click.option('-p', '--plot', is_flag=True, help='Plot results')
@click.pass_obj
def roll(obj: Dict[str, Union[bool, str]], **options: Dict[str, str]) -> None:
    commands.command_roll(**obj, **options)

@main.command(help='Collect a trace with triggering')
@click.option('-n', '--record-length', default=5, help='Specify number of reads after the trigger', metavar='<num-reads>')
@click.option('-r', '--measurement-duration', default=1000, help='Specify time range to read over', metavar='<microseconds>')
@click.option('-p', '--plot', is_flag=True, help='Plot results')
@click.pass_obj
def trigger(obj: Dict[str, Union[bool, str]], **options: Dict[str, str]) -> None:
    commands.command_trigger(**obj, **options)

if __name__ == '__main__':
    main()
