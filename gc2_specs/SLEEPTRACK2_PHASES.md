At every phase, we would:
* Test circuit on breadboard, using the breakout board
* Design and manufacture PCB

Phase 1
=======
Battery charging circuit and photon only. All pins of the photon will be exposed using 0.1" spaced through-holes, so that headers can be soldered on. In the very final version, these through-holes will not be present.

Chips:
* Headerless Particle Photon
* MCP73831
* MAX17043

Connectors:
* On/off switch to open/close battery circuit.
* EMG connector.

Reference circuit:
* https://cdn.sparkfun.com/datasheets/IoT/Photon_Battery_Shield_v10.pdf
* In the above circuit, no need for the barrel connector,
* No need for the alert pin

Is it possible to have this board have the same footprint as the sleeptrack1 board ?

Phase 2
=======
Everything from phase 1 + IMU chips

Chips:
* BNO055 on the sleeptrack board
* LSM9DS1 on a panneled small external board

Reference circuit:
* BNO055: https://cdn-learn.adafruit.com/assets/assets/000/024/546/original/sensors_BNO055_REV-C.png?1429569060

Phase 3
=======
Everything from phase 2 + audio circuits

Chips:
* AD5220
* TPA2005D1DRB

Components
* SJ1-3515N
* SDR08540M3-01

Phase 4
=======
Everything from phase 3 + LED bar-chart

Chips:
* LM3914

Datasheets:
* LM3914: http://www.ti.com/lit/ds/symlink/lm3914.pdf

Phase 5
=======
Everything from phase 4 + vibration motor driver
