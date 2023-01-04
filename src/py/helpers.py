import sys
from pathlib import Path
from typing import List
from click import secho

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

def peek(results_t: List[int], results_v: List[float]) -> None:

    secho('> Summarizing first five reads from device', fg='yellow')
    secho('> {:<11} {}'.format('Time (us)', 'Volts (V)'), fg='blue')
    secho('> -----------------', fg='blue')

    for t, v in zip(results_t[:5], results_v[:5]):
        secho('  {:<11} {}'.format(t, v), fg='blue')

def plot(results_t: List[int], results_v: List[float], save_path: str) -> None:

    secho(f'> Exporting plot to {save_path}', fg='yellow')
    save_path = Path(save_path)

    if not save_path.parent.exists():
        sys.exit(f'Path {save_path.parent} does not exist!')

    # Lazy load
    import matplotlib.pyplot as plt

    plt.style.use('dark_background')
    plt.minorticks_on()
    plt.grid(color='navy', lw=0.5, which='major')
    plt.grid(color='navy', lw=0.25, which='minor')
    plt.xlabel('Time ($\mu$s)')
    plt.ylabel('Voltage (V)')
    plt.plot(results_t, results_v, color='greenyellow', lw=0.75)
    plt.savefig(save_path, dpi=500)
