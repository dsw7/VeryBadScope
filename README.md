# VeryBadScope
Actually, a surprisingly good oscilloscope given the low cost of the hardware!
## Table of Contents
- [Setup](#setup)
    - [Hardware requirements](#hardware-requirements)
    - [Python requirements](#python-requirements)
    - [Uploading code to device](#uploading-code-to-device)
    - [Handshaking with the device](#handshaking-with-the-device)
- [Usage](#usage)
    - [Setting up the hardware](#setting-up-the-hardware)
    - [Capturing a trace](#capturing-a-trace)
    - [Visualizing the results](#visualizing-the-results)
    - [Roll mode](#roll-mode)
    - [Triggering](#Triggering)
      - [Edge triggering](#edge-triggering)
      - [Level triggering](#level-triggering)
      - [Exact level triggering](#exact-level-triggering)
    - [Exporting data for analysis](#exporting-data-for-analysis)
- [Is this product reliable?](#is-this-product-reliable)
    - [Manual testing](#manual-testing)
    - [Testing against Rigol DS1054Z](#testing-against-rigol-ds1054z)
- [Testing](#testing)
- [Limitations of this device](#limitations-of-this-device)

## Setup

### Hardware requirements
While optional, it is strongly recommended to install [arduino-cli](https://github.com/arduino/arduino-cli).
This project's build system is based upon `arduino-cli`. To install `arduino-cli`, follow the
[Quickstart](https://github.com/arduino/arduino-cli#quickstart) section provided by the `arduino-cli`
developers. Alternatively, the `src/ino/ino.ino` "sketch" can be manually uploaded to the device via the
Arduino IDE.

If using `arduino-cli`, ensure that the Fully Qualified Board Name (FQBN) variable is updated in the
`Makefile`. This project was built atop an Uno which is reflected in the `Makefile`. If not working with an
Uno, change the FQBN as follows:
```
sed -i 's/FULLY_QUALIFIED_BOARD_NAME = arduino:avr:uno/FULLY_QUALIFIED_BOARD_NAME = <your-fqbn>/' Makefile
```

### Python requirements
This project's command line interface is written in Python and uses several Python libraries. These libraries
must be installed. To install the libraries:
```
python3 -m pip install --user --requirement requirements.txt
```
Note that these requirements only cover the CLI. Additional requirements are required to run the unit tests,
if one chooses to run the tests (see [Testing](#testing)).

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
  <img width="400" src=docs/example_555_timer_on_breadboard.png>
</p>

The VCC and ground power rails were connected to the device's 5V and GND pins, and the output under study (pin
3 in this configuration) was connected to pin A0 (the white jumper).

### Capturing a trace
To capture a trace, run:
```
python3 src/py/runner.py roll -n 100 -r 1000000
```
In this example, the device is asked to collect a trace with a record length of 100 (i.e. 100 "reads") over a
span of 1,000,000 microseconds, or 1 second (the measurement duration). This evaluates to approximately 10,000
microseconds between a read. Upon dispatch, the command will return:
```
> Reading data from device
```
The device will now acquire data. After both data acquisition and transmission are complete, the command will
print the total round trip time to the console:
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
python3 src/py/runner.py roll -n 200 -r 1200000 -p -q /tmp/example_roll.png
```
Or using long options:
```
python3 src/py/runner.py roll -n 200 -r 1200000 --plot --plot-path /tmp/example_roll.png
```
Which will generate the following plot:
<p align="center">
  <img src=docs/example_roll.png>
</p>

### Roll mode
Both the [Capturing a trace](#capturing-a-trace) and [Visualizing the results](#visualizing-the-results)
sections depicted the use of the `roll` command as examples. In "roll mode," no triggering occurs. The device
simply captures the first `-n` or `--record-length` number of reads and transmits this data to the host upon
read termination. This mode is useful for probing the characteristics of a waveform of interest such that an
appropriate trigger type and level (or delta) can be chosen. This section segues very nicely to the next
section: [Triggering](#triggering).

### Triggering
An oscilloscope is not complete without triggering. As such, this software comes with several trigger types:

#### Edge triggering
Pulse waves are commonly encountered in digital electronics. Once a wave has been identified as being either
rectangular or square in nature, a user may set an edge trigger on the wave in order to capture a "waveform."
The `edge` command comes in handy here:
```
python3 src/py/runner.py edge -n 200 -r 1200000 -p -q /tmp/example_edge_rising.png
```
In this mode, data acquisition began immediately upon the sharp transition of the wave from 0V to >4V:
<p align="center">
  <img src=docs/example_edge_rising.png>
</p>

By default, the `edge` command triggers on a rising edge. To trigger on a falling edge, pass the `falling`
argument to `-t` or `--trigger` instead:
```
python3 src/py/runner.py edge -n 200 -r 1200000 -t falling -p -q /tmp/example_edge_falling.png
```
In this case, data acquisition began immediately upon the transition of the wave from >4V to 0V:
<p align="center">
  <img src=docs/example_edge_falling.png>
</p>

The transition that forces the trigger is termed the "delta" and can be specified via the `-d` or `--delta`
argument. Note that the delta is relative - a transition for 5V to 1.5V on a falling edge trigger will result
in data acquisition with `--delta=3`, for example, given that the difference between 5V and 1.5V exceeds the
delta. The device will wait indefinitely for a trigger so an appropriate delta must be chosen.

#### Level triggering
Many waves do not alternate between fixed minima and maxima - sinusoids and decaying exponentials immediately
come to mind. Such waves do not possess an "edge" that can be used as a trigger. In this case, a level must
instead be used as a trigger. First, start by dispatching a `roll` command in order to study the wave:
<p align="center">
  <img src=docs/example_roll_charging_discharging.png>
</p>

Given this knowledge in hand, one can now trigger the data acquisition when the voltage reads 2V:
```
python3 src/py/runner.py level -n 200 -r 1200000 -l 2 -p -q /tmp/example_level_rising.png
```
Now the voltage intercept is approximately 2V:
<p align="center">
  <img src=docs/example_level_rising.png>
</p>

Note that the trigger occurred on a rising voltage. The trigger can instead be set on a falling voltage:
```
python3 src/py/runner.py level -n 200 -r 1200000 -t falling -l 2 -p -q /tmp/example_level_falling.png
```
Again, the voltage intercept is approximately 2V, but in this case the voltage falls post-trigger:
<p align="center">
  <img src=docs/example_level_falling.png>
</p>

#### Exact level triggering
It is difficult to trigger at exactly a voltage level set via `--level`, essentially owing to the 4
microsecond resolution associated with the
[micros()](https://www.arduino.cc/reference/en/language/functions/time/micros/) call which this software uses
alongside [analogRead()](https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/) to
acquire data. The uncertainly about the trigger level is termed the "epsilon" and can be specified using the
`-e` or `--epsilon` argument. The absolute minimum $\epsilon$ is 0.025V and using such a small value may lead to
non-triggering. As such, it is essential to experiment with the $\epsilon$ value if a wave cannot be triggered
under any circumstances. Generally, a trigger $t$ will occur "around" the trigger level $L$, that is,

$$
L - \epsilon \leq t \leq L + \epsilon
$$

For example, consider this wave triggered at `--level=2` and `--epsilon=0.075`:
<p align="center">
  <img src=docs/example_level_epsilon.png>
</p>

Triggering occurred at approximately $L + \epsilon$ and in this case a smaller value for $\epsilon$ could be
chosen, owing to the large number of reads (50 in this case) relative to the short measurement duration (10
ms). A larger $\epsilon$ must be chosen for a decreasing record-length to measurement duration. Additionally,
$\epsilon$ must not be set to too small a value, otherwise triggering may occur on noise in a noisy signal.

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

### Manual testing
This product was tested using the classic 555 timer ([TI
NE555P](https://www.ti.com/product/NE555/part-details/NE555P) to be exact) wired in an astable multivibrator
configuration. The circuit follows:
<p align="center">
  <img width="200" src=docs/example_555_timer.png>
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

To benchmark this device, a 555 timer circuit was constructed with $R_1$ and $C$ held constant, at 99.8
$k\Omega$ and 1 $\mu F$, respectively. Traces were collected for varying $R_2$ values and for each trace, the
duty cycle $D_a$ was computed. $D_a$ was then compared against the theoretical duty cycle, $D_e$:

$$
D_e = 100\times(\frac{99.8 + R_2}{99.8 + 2R_2})
$$

Each trace consisted of a record length of 250 and a measurement duration of 250 milliseconds. The raw data
(`benchmark/*raw.csv`) was clipped to consist of only one period (`benchmark/*clipped.csv`) with clipping
starting on the first rising edge and ending on the next rising edge. Benchmarking was performed by observing
the duty cycle as opposed to the output frequency simply because the duty cycle does not depend on the
capacitance $C$, and it is very easy to accurately measure the resistances $R_1$ and $R_2$. The results of the
benchmark follow:

| $R_1\ (k\Omega$) | $R_2\ (k\Omega$) | $C\ (\mu F)$ | $D_a$ | $D_e$ |
| ---------------- | ---------------- | ------------ | ----- | ----- |
| 99.8             | 9.82             | 1            | 91.7  | 91.8  |
| 99.8             | 14.48            | 1            | 88.9  | 88.8  |
| 99.8             | 21.6             | 1            | 84.7  | 84.9  |
| 99.8             | 26.27            | 1            | 82.2  | 82.8  |
| 99.8             | 31.5             | 1            | 80.6  | 80.7  |
| 99.8             | 36.0             | 1            | 79.1  | 79.1  |
| 99.8             | 46.2             | 1            | 75.9  | 76.0  |

In graphical form:
<p align="center">
  <img width="720" src=benchmark/benchmark.png>
</p>

From this analysis, we can see that the observed duty cycles are remarkably consistent with their theoretical
counterparts, indicating that this device is capable of surprisingly accurate measurement.

### Testing against Rigol DS1054Z
As before, a 555 timer was set up in an astable multivibrator configuration. $R_1$ and $R_2$ were 99.8 and
21.6 $k\Omega$, respectively. $C$ was 0.47 $\mu F$. The output was a 19.7 Hz rectangular wave as measured
using a Rigol DS1054Z oscilloscope:
<p align="center">
  <img src=benchmark/rigol/rigol_1054z.png>
</p>

The DS1054Z was set to 10 ms per division with falling edge triggering. Identical conditions were used to
measure the output wave using this software. The output follows:
<p align="center">
  <img src=benchmark/rigol/very_bad_scope.png>
</p>

The rising edge-to-rising edge period was measured at 50.42 milliseconds by inspecting a CSV corresponding to
the above trace. This period represents a 0.554% difference from the 50.70 millisecond period obtained from
the Rigol DS1054Z.

## Testing
To run unit tests, run the following `make` target:
```
make test SERIAL_PORT=<serial-port>
```
Note that this assumes that `pytest` is installed.

## Limitations of this device
The ATmega328 microcontroller comes with only 2048 bytes of static RAM. This is a very small amount, and the
majority of this space is used to store waveform data. In order to maximize the amount of space available,
this software moves most string literals to FLASH memory on start using the `F()` macro. Even with this
optimization, the device is limited to a record length of about 250 reads. Attempting to load the waveform
buffer with any more reads may begin to yield some anomalous behavior. For example, running a `roll` command
with `--debug` shows:
```
$ python3 src/py/runner.py --debug roll -n 250 -r 250000
> Reading data from device
Connecting using parameters:
{
    "baudrate": 19200,
    "parity": "N",
    "stopbits": 1,
    "bytesize": 8,
    "timeout": 5,
    "port": "/dev/ttyS2"
}
DTR (Data Terminal Ready) was sent. Waiting for device to reset
Sending message: "roll:250:250000"
Sent 15 bytes
Waiting to receive message...
Received message: b'1;1023 1023 1023 1023 1023 1021 1017 101'...
...
```
This is a normal result. But at 270 reads:
```
$ python3 src/py/runner.py --debug roll -n 270 -r 250000
> Reading data from device
Connecting using parameters:
{
    "baudrate": 19200,
    "parity": "N",
    "stopbits": 1,
    "bytesize": 8,
    "timeout": 5,
    "port": "/dev/ttyS2"
}
DTR (Data Terminal Ready) was sent. Waiting for device to reset
Sending message: "roll:270:250000"
Sent 15 bytes
Waiting to receive message...
Received message: b'p\xed1;1023 1023 1023 1023 1023 1021 1017 1'...
...
An exception occurred when decoding results: "'utf-8' codec can't decode byte 0xed in position 1: invalid continuation byte"
```
Note the unusual byte that was received. By 280 reads the device can no longer return valid data:
```
$ python3 src/py/runner.py --debug roll -n 280 -r 250000
> Reading data from device
2023-01-12 02:40:42.811 Connecting using parameters:
{
    "baudrate": 19200,
    "parity": "N",
    "stopbits": 1,
    "bytesize": 8,
    "timeout": 5,
    "port": "/dev/ttyS2"
}
2023-01-12 02:40:42.828 DTR (Data Terminal Ready) was sent. Waiting for device to reset
2023-01-12 02:40:44.834 Sending message: "roll:280:250000"
2023-01-12 02:40:44.835 Sent 15 bytes
2023-01-12 02:40:44.835 Waiting to receive message...
2023-01-12 02:40:50.145 Received message: b'\x00\xff1;0 0 0 0 0 0 0 0 \x0e\x8e\x01o\xb9V\x0f\x0f\x02\x90\x02\xbe\x03\x0f\x02\x0f\x02k\x00\x0f'
...
An exception occurred when decoding results: "'utf-8' codec can't decode byte 0xff in position 1: invalid start byte"
```
