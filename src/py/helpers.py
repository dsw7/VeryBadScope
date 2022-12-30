from typing import List
from click import secho

def results_to_ints(results: str) -> List[int]:
    return [int(r) for r in results.strip().split()]

def normalize_time_series(series: List[int]) -> List[int]:
    return [t - series[0] for t in series]

def peek(results_t: List[int], results_v: List[int]) -> None:

    secho('> Summarizing first five reads from device', fg='yellow')
    secho('> {:<11} {}'.format('Time (us)', 'Volts'), fg='blue')
    secho('> -----------------', fg='blue')

    for t, v in zip(results_t[:5], results_v[:5]):
        secho('  {:<11} {}'.format(t, v), fg='blue')
