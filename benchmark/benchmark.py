import matplotlib.pyplot as plt
from numpy import linspace

FILES_TO_ANALYZE = [
    '9_82_clipped.csv', '14_48_clipped.csv', '21_6_clipped.csv', '26_27_clipped.csv',
    '31_5_clipped.csv', '36_0_clipped.csv', '46_2_clipped.csv'
]
PLOT_NAME = 'benchmark.png'
R2_LINSPACE = linspace(5, 50)
R1 = 99.8
R2_VALUES = [9.82, 14.48, 21.6, 26.27, 31.5, 36.0, 46.2]

def get_duty_cycle(csvfile: str) -> float:
    with open(csvfile) as f:
        data = f.readlines()

    data = [d.strip().split(',') for d in data[1:]]

    times = [int(d[0]) for d in data]
    voltages = [float(d[1]) for d in data]

    idx = 0
    for u, v in enumerate(voltages):
        if v < 0.00001:
            idx = u
            break

    period = times[-1] - times[0]
    high_t = times[idx - 1] - times[0]
    return round(high_t * 100 / period, 1)

def get_theoretical(r: float) -> float:
    return 100 * ((R1 + r) / (R1 + 2 * r))

def main() -> None:
    observed_duty_cycles = [get_duty_cycle(f) for f in FILES_TO_ANALYZE]
    plt.scatter(R2_VALUES, observed_duty_cycles, marker='o', color='k', label='$D_a$')

    expected_duty_cycles = [get_theoretical(t) for t in R2_LINSPACE]
    plt.plot(R2_LINSPACE, expected_duty_cycles, ls='--', label='$D_e$')

    plt.legend(fontsize=15)
    plt.xlabel('$R_2\ (k\Omega)$', fontsize=15)
    plt.ylabel('$D$', fontsize=15)
    plt.savefig(PLOT_NAME)

if __name__ == '__main__':
    main()
