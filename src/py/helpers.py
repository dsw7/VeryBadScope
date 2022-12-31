from tempfile import gettempdir
from os import path
from typing import List
from click import secho

CONST_VOUT_INO = 5 / 1023

def results_to_ints(results: str) -> List[int]:
    return [int(r) for r in results.strip().split()]

def normalize_time_series(series: List[int]) -> List[int]:
    return [t - series[0] for t in series]

def normalize_voltage_series(series: List[int]) -> List[int]:
    return [round(v * CONST_VOUT_INO, 3) for v in series]

def peek(results_t: List[int], results_v: List[float]) -> None:

    secho('> Summarizing first five reads from device', fg='yellow')
    secho('> {:<11} {}'.format('Time (us)', 'Volts (V)'), fg='blue')
    secho('> -----------------', fg='blue')

    for t, v in zip(results_t[:5], results_v[:5]):
        secho('  {:<11} {}'.format(t, v), fg='blue')

def plot(results_t: List[int], results_v: List[float]) -> None:

    save_path = path.join(gettempdir(), 'test.png')
    secho(f'> Exporting plot to {save_path}', fg='yellow')

    # Lazy load
    import matplotlib.pyplot as plt

    plt.style.use('dark_background')
    plt.minorticks_on()
    plt.grid(color='navy', lw=0.5)
    plt.xlabel('Time ($\mu$s)')
    plt.ylabel('Voltage (V)')
    plt.plot(results_t, results_v, color='greenyellow', lw=0.75)
    plt.savefig(save_path, dpi=500)
