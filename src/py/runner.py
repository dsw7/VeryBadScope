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

@main.command(help='Collect a trace')
@click.option('-n', '--count', default=5, help='Specify number of reads', metavar='<num-reads>')
@click.option('-r', '--time-range', default=1000, help='Specify time range to read over', metavar='<microseconds>')
@click.pass_obj
def read(obj: Dict[str, Union[bool, str]], **options: Dict[str, str]) -> None:
    commands.command_read(**obj, **options)

if __name__ == '__main__':
    main()
