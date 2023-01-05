import sys
from pathlib import Path
from abc import ABC, abstractmethod
from typing import Dict, Union, TypeVar, List
from click import secho

T = TypeVar('T')
ANALOG_TO_VOLT = 5 / 1023
VOLT_TO_ANALOG = 1023 / 5

def results_to_ints(results: str) -> List[int]:
    return [int(r) for r in results.strip().split()]

def normalize_time_series(series: List[int]) -> List[int]:
    return [t - series[0] for t in series]

def normalize_voltage_series(series: List[int]) -> List[int]:
    return [round(v * ANALOG_TO_VOLT, 3) for v in series]

def volt_to_analog(voltage: float) -> int:
    return int(VOLT_TO_ANALOG * voltage)


class CommandBase(ABC):

    def __init__(self: T, **cli_params: Dict[str, Union[bool, str]]) -> T:

        self.cli_params = cli_params
        self.raw_t = None
        self.raw_v = None
        self.normalized_t = None
        self.normalized_v = None

    @abstractmethod
    def acquire_data(self: T) -> None:
        pass

    def transform_data(self: T) -> None:
        self.normalized_t = normalize_time_series(results_to_ints(self.raw_t))
        self.normalized_v = normalize_voltage_series(results_to_ints(self.raw_v))

    def to_csv(self: T) -> None:

        save_path = Path(self.cli_params['csv_path'])
        secho(f'> Exporting CSV to {save_path}', fg='yellow')

        if not save_path.parent.exists():
            sys.exit(f'Path {save_path.parent} does not exist!')

        with open(save_path, 'w') as f:
            f.write('Time (microseconds),Voltage (volts)\n')
            for t, v in zip(self.normalized_t, self.normalized_v):
                f.write(f'{t},{v}\n')

    def plot(self: T) -> None:

        save_path = Path(self.cli_params['plot_path'])
        secho(f'> Exporting plot to {save_path}', fg='yellow')

        if not save_path.parent.exists():
            sys.exit(f'Path {save_path.parent} does not exist!')

        import matplotlib.pyplot as plt  # Lazy load for performance reasons

        plt.style.use('dark_background')
        plt.minorticks_on()
        plt.grid(color='navy', lw=0.5, which='major')
        plt.grid(color='navy', lw=0.25, which='minor')
        plt.xlabel(r'Time ($\mu$s)')
        plt.ylabel('Voltage (V)')
        plt.plot(self.normalized_t, self.normalized_v, color='greenyellow', lw=0.75)
        plt.savefig(save_path, dpi=500)

    def peek(self: T) -> None:

        secho('> Summarizing first five reads from device', fg='yellow')
        secho('> {:<11} {}'.format('Time (us)', 'Volts (V)'), fg='blue')
        secho('> -----------------', fg='blue')

        for t, v in zip(self.normalized_t[:5], self.normalized_v[:5]):
            secho('  {:<11} {}'.format(t, v), fg='blue')

    def export_data(self: T) -> None:

        if self.cli_params['to_csv']:
            self.to_csv()

        if self.cli_params['plot']:
            self.plot()
        else:
            self.peek()

    def main(self: T) -> None:
        self.acquire_data()
        self.transform_data()
        self.export_data()
