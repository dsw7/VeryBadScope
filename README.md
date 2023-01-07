# VeryBadScope
A very bad oscilloscope!
## Table of Contents
- [Setup](#setup)
    - [Uploading code to device](#uploading-code-to-device)
    - [Handshaking with the device](#handshaking-with-the-device)
- [Usage](#usage)
    - [Setting up the hardware](#setting-up-the-hardware)
    - [Capturing a trace](#capturing-a-trace)
    - [Visualizing the results](#visualizing-the-results)
    - [Edge triggering](#edge-triggering)
    - [Roll mode](#roll-mode)
    - [Exporting data for analysis](#exporting-data-for-analysis)
- [Is this product reliable?](#is-this-product-reliable)

## Setup

### Uploading code to device
The code located in this repository must be uploaded to the device. To upload the code, run the `upload`
`make` target:
```
make upload SERIAL_PORT=<serial-port>
```
The `<serial-port>` argument can be either a COM port (if running from a Windows based host) or a device file
(if running from a Linux based host). In general, a serial port `COM<n>` maps to a device file `/dev/ttyS<n -
1>`. For example, `COM3` would map to `/dev/ttyS2`. This mapping may be relevant if running this program on
Cygwin. If the code uploads without error, proceed to the [Handshaking with the
device](#handshaking-with-the-device) step.

### Handshaking with the device
Run a handshake to confirm that the device and the host can communicate. To handshake, run:
```
python3 src/py/runner.py hello
> Handshaking with device
```
If the device and host can communicate, the device will return the message:
```
> Received message from device: "Hello from InoDAQ2. I should blink 5 times!"
```
And the device's onboard LED will blink 5 times in rapid succession.

## Usage
All instructions herein assume that the sketch and associated C++ files have already been uploaded to the
device. For more information, see [Uploading code to device](#uploading-code-to-device).

### Setting up the hardware
This device should be used to probe low power circuits only. To probe a circuit, connect the circuit's VCC to
the device's 5V pin and connect the circuit's ground to the device's ground. Then connect the output to pin
A0. For example, consider the following multivibrator:
<p align="center">
  <img width="400" src=./docs/example_circuit.png>
</p>

The VCC and ground power rails were connected to the device's 5V and GND pins, and the output under study (pin
3 in this configuration) was connected to pin A0 (the white jumper).

### Capturing a trace
To capture a trace, run:
```
python3 src/py/runner.py edge -n 100 -r 1000000
```
In this example, the device was asked to collect a trace with a record length of 100 (i.e. 100 "reads") over a
span of 1,000,000 microseconds, or 1 second (the measurement duration). This evaluates to approximately 10,000
microseconds between a read. Upon dispatch, the command will return:
```
> Waiting on trigger...
```
The device waits for a trigger before beginning data acquisition. Once triggered, the device acquires the data
and sends the data to the host via UART. Data acquisition and transmission is complete when the rount trip
time is printed to the console:
```
> Round trip time: 1944.3388 ms
```
In this example, the round trip time accounts for both the measurement duration (1 second), and the time
required to transmit the acquired data from the device to the host over a slow, serial connection
(approximately 1 second). Once the round trip is complete, the device will summarize the first five reads as a
sanity check:
```
> Summarizing first five reads from device
> Time (us)   Volts (V)
> -----------------
  0           4.731
  10056       4.682
  20120       4.726
  30184       4.692
  40248       4.697
```

### Visualizing the results
Peeking at the results, as was described in the [Capturing a trace](#capturing-a-trace) section, is relatively
uninteresting. To actually visualize the results, the software can be coerced into exporting a plot by
dispatching the runner with additional plotting arguments:
```
python3 src/py/runner.py edge -n 200 -r 1200000 -p -q /tmp/example_plotting.png
```
Or using long options:
```
python3 src/py/runner.py edge -n 200 -r 1200000 --plot --plot-path /tmp/example_plotting.png
```
Which will generate the following plot:
<p align="center">
  <img src=./docs/example_plotting.png>
</p>

### Edge triggering
By default, the `edge` command triggers on a rising edge. To trigger on a falling edge, pass the `falling`
argument to `--trigger` instead:
```
python3 src/py/runner.py edge -n 200 -r 1200000 --trigger=falling -p -q /tmp/example_plotting_falling.png`
```
In edge triggered mode, the device will begin data acquisition when a sufficient delta exists between two
reads. This delta is 4 volts by default. On a falling edge trigger, note that data acquisition began only
after the voltage drop exceeded 4 volts between two reads:
<p align="center">
  <img src=./docs/example_plotting_falling.png>
</p>

The trigger delta can be adjusted using the `-d` or `--delta` argument. The device will wait indefinitely for
a trigger so an appropriate delta must be chosen.

### Roll mode
This software comes packaged with an additional command: "roll mode." In this mode, no triggering occurs. The
device simply captures the first `-n` or `--record-length` number of reads and transmits this data to the host
upon read termination. For example:
```
python3 src/py/runner.py roll -n 200 -r 1200000 -p -q /tmp/example_roll.png
```
Returns:
<p align="center">
  <img src=./docs/example_roll.png>
</p>

This mode is useful for probing the characteristics of a waveform of interest such that an appropriate trigger
type and level (or delta) can be chosen.

### Exporting data for analysis
For more accurate analysis, the data collected by the device can be exported to CSV format. For example, to
export 10 reads that were gathered over a duration of 0.1 seconds:
```
python3 src/py/runner.py roll -n 10 -r 100000 --to-csv
```
Which will print:
```
> Reading data from device
> Round trip time: 195.7858 ms
> Exporting CSV to /tmp/roll.csv
> Summarizing first five reads from device
> Time (us)   Volts (V)
> -----------------
  0           5.0
  10064       4.897
  20128       4.848
  30184       4.868
  40240       4.707
```
The contents of `/tmp/roll.csv` follow:
```
Time (microseconds),Voltage (volts)
0,5.0
10064,4.897
20128,4.848
30184,4.868
40240,4.707
50304,4.741
60368,4.736
70432,4.682
80496,4.721
90552,4.692
```
A custom CSV export path can be specified via `--csv-path`.

## Is this product reliable?
This product was tested using the classic 555 timer ([TI
NE555P](https://www.ti.com/product/NE555/part-details/NE555P) to be exact) wired in an astable multivibrator
configuration. The circuit follows:
<p align="center">
  <img width="200" src=./docs/555_timer_200.png>
</p>

This configuration consists of the 555 timer itself, two resistors, and a capacitor. The resistances of the
two resistors, $R_1$ and $R_2$, and the capacitance of the capacitor, $C$, serve as inputs into this system.
These values can be adjusted in order to tune the frequency and duty cycle of the output: a PWM signal. In an
astable configuration, the frequency $f$ is given by:

$$
f = \frac{1.44}{C(R_1 + 2R_2)}
$$

And the duty cycle $D$ is given by:

$$
D = \frac{R_1 + R_2}{R_1 + 2R_2}
$$

The values for $R_1$, $R_2$ and $C$ were measured for the above circuit and follow:

| Component | Value           | Value in SI units         |
|:---------:|-----------------|---------------------------|
| $R_1$     | 99.8 $k\Omega$  | 99800.0 $\Omega$          |
| $R_2$     | 21.60 $k\Omega$ | 21600.00 $\Omega$         |
| $C$       | 9.52 $\mu F$    | 9.52 $\times$ $10^6$ $F$  |

Inputting these values into the definition of $f$ yields a theoretical frequency of 1.06 $s^{-1}$.
Correspondingly, the theoretical duty cycle $D$ is 0.849. These values align reasonably with the actual output
of this device:

<p align="center">
  <img width="650" src=./docs/example.png>
</p>
