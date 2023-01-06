# VeryBadScope
A very bad oscilloscope!
## Table of Contents
- [Setup](#setup)
    - [Uploading code to device](#uploading-code-to-device)
    - [Handshaking with the device](#handshaking-with-the-device)
- [Usage](#usage)
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
$ python3 src/py/runner.py hello
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
