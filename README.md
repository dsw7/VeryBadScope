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
  <img width="250" src=./docs/raw_circuit.jpg>
</p>

This configuration consists of the 555 timer itself, two resistors, $$R_1$$ and $$R_2$$, and a capacitor $$C$$.
The resistances $$R_1$$, $$R_2$$ and the capacitance $$C$$ serve as inputs into this system. These values can
be adjusted to in order to tune the frequency and duty cycle of the output: a PWM signal. In an astable configuration,
the frequency $$f$$ is given by:

$$
f = \frac{1.44}{C(R_1 + 2R_2)}
$$

And the duty cycle $$D$$ is given by:

$$
D = \frac{R_1 + R_2}{R_1 + 2R_2}
$$
