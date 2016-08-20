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
Everything from phase 1 + BNO055 chip

Chips:
* BNO055 on the sleeptrack board

Reference circuit:
* BNO055: https://cdn-learn.adafruit.com/assets/assets/000/024/546/original/sensors_BNO055_REV-C.png?1429569060
* BNO055 datasheet: https://cdn-shop.adafruit.com/datasheets/BST_BNO055_DS000_12.pdf
* No need for the 5V/3.3V level switching circuit as our board is 3.3V only

Fixes:
* Add I2C (SDA, SCL) pull-up resistors.
* Flip the pins on the EMG connector so that they mirror the layout on the EMG sensor when put face to face.
* Put diode back in charging circuit.
* See whether charging LED is behaving correctly.

Questions:
* is it possible to put the chip and associated components on the back of the board ?

Phase 3
=======
Everything from phase 3 + BNO055 on panelized board

Chips
* BNO055 on a panelized small external board (each "device" will come with two BNO055 chips, one on the main sleeptrack board, one on the external IMU board which will snap away after assembly)
Components
* 2 "face mounted" push buttons
* SDR08540M3-01 speaker
  * POS directly connected to the Photon A4 pin, NEG directly connected to GND.

The footprint for the BNO055 should be modified to allow more easy debugging. The pads will be longer and will be completed by vias. Not all pins need to have that treatment, the ones that aren't connected inside the chip can be ignored.

This has the "chin sensor" board whose purpose is to detect movements of the lower jaw independently of the rest of the head. It contains the BNO055 circuit along with chip and connects to the main board through a cable with 4 conductors (3V3, GND, SDA, SCL). The circuit is the same as in phase 2, except **COM3** needs to be **high**.

In this phase i'd also like to add 2 push buttons for controlling the device as well as the speaker to allow feedback to the user more easily. The speaker can be connected directly to the photon A4 pin.

Reference circuit:
* BNO055: https://cdn-learn.adafruit.com/assets/assets/000/024/546/original/sensors_BNO055_REV-C.png?1429569060
* BNO055 datasheet: https://cdn-shop.adafruit.com/datasheets/BST_BNO055_DS000_12.pdf

Phase 4
=======
Everything from phase 3 + audio circuits

Chips:
* AD5220
* TPA2005D1DRB

Components
* SJ1-3515N
* SDR08540M3-01

Phase 5
=======
Everything from phase 4 + LED bar-chart

Chips:
* LM3914

Datasheets:
* LM3914: http://www.ti.com/lit/ds/symlink/lm3914.pdf

Phase 6
=======
Everything from phase 5 + vibration motor driver
