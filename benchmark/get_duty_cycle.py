# R1 is 99.8 kOhm
# R2s are located in the filename -> 9_82_clipped.csv means 9.82 kOhm
# "Clipped" means data was between the first rising edge and the next rising edge
# C was 1 uF
# Read 250 times over 250000 usec

import matplotlib.pyplot as plt
from numpy import linspace

FILES_TO_ANALYZE = [
    '9_82_clipped.csv',
    '14_48_clipped.csv',
    '21_6_clipped.csv',
    '26_27_clipped.csv',
    '31_5_clipped.csv',
    '36_0_clipped.csv',
    '46_2_clipped.csv'
]

PLOT_WIDTH_INCHES = 6
PLOT_HEIGHT_INCHES = 4
PLOT_DPI = 80

R1 = 99.8
R2_VALUES = [9.82, 14.48, 21.6, 26.27, 31.5, 36.0, 46.2]
# EXPECTED_DUTY_CYCLES = [91.8, 88.8, 84.9, 82.8, 80.7, 79.1, 76.0]

def get_duty_cycle(csvfile: str) -> float:

    with open(csvfile) as f:
        data = f.readlines()

    data = [d.strip().split(',') for d in data[1:]]

    times = [int(d[0]) for d in data]
    voltages = [float(d[1]) for d in data]

    period = times[-1] - times[0]

    idx = 0
    for u, v in enumerate(voltages):
        if v < 0.00001:
            idx = u
            break

    high_t = times[idx - 1] - times[0]

    return round(high_t * 100 / period, 1)

def get_theoretical(r: float) -> float:
    return 100 * ((R1 + r) / (R1 + 2 * r))

def main() -> None:
    plt.rcParams['axes.spines.right'] = False
    plt.rcParams['axes.spines.top'] = False
    plt.rcParams.update({
        'text.usetex': True,
        'font.family': 'sans-serif',
        'font.sans-serif': 'Helvetica',
    })

    times = linspace(5, 50)

    observed_duty_cycles = [get_duty_cycle(f) for f in FILES_TO_ANALYZE]
    expected_duty_cycles = [get_theoretical(t) for t in times]

    plt.figure(figsize=(PLOT_WIDTH_INCHES, PLOT_HEIGHT_INCHES), dpi=PLOT_DPI)
    plt.plot(times, expected_duty_cycles, ls='--')
    plt.scatter(R2_VALUES, observed_duty_cycles, marker='o', color='k')
    plt.xlabel('$R_2\ (k\Omega)$', fontsize=12)
    plt.ylabel('$D$', fontsize=12)
    plt.savefig('benchmark.png')

if __name__ == '__main__':
    main()
