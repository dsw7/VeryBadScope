# VeryBadScope
A very bad oscilloscope!
## Table of Contents
- [Setup](#setup)
- [Usage](#usage)
- [Is this product reliable?](#is-this-product-reliable)

## Setup

## Usage

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
