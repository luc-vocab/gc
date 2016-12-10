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
Everything from phase 3 + vibration motor circuit + other improvements.

The board will consist of 3 panneled boards:
* **Main Board**: contains the Photon and other components
* **Host board for BNO055 Nano breakout (2x)**: allows us to solder the nano bno055 breakout

Let's try to get our board design and BOM in the right format for assembly by tempo automation.

**See below for description of the 3 parts:**

* **Main Board**
 * See whether the photon through-holes can be placed lined up exactly with holes in our board, to retain the ability to use female headers without sacrificing space.
 * Replace battery charging LED with a more appropriate one
 * Remove buttons
 * Remove the 3x 0.1" pitch EMG connector, it's not needed
 * Add 2 4x 0.1" pitch I2C connectors for connecting to both IMU boards.
 * Add vibrator motor circuit using the DRV2605L (SSOP package). Se below for sample circuit and pin assignments.
 * remove the bno055 circuit. I thought the main board needed it, but I think it's only needed on the auxiliary IMU board.
 * See how small we can make the main board. Ignore previous constraints about connector placement initially. The battery's dimensions are *2.00 x 1.32 x 0.23" (50.8 x 33.5 x 5.9 mm)*. Can our board's dimensions approach this ?
* **Host boards for the BNO055 nano breakout (2x)**
 * Add a two "host" board fors this one, adding 4x 0.1" pitch connector (throughole), as well as an easy way to change the ADO level (I2C address selection). Each board should use a different I2C address.: https://www.tindie.com/products/onehorse/wearable-bno055-nano-board/



**Vibration circuit details:**

http://www.ti.com/lit/ds/symlink/drv2605l.pdf page 52 / 9.1 has a sample circuit (figure 58).

* Connect SDA to D0
* Connect SCL to D1
* Connect IN/TRIG to the Photon's A1 pin
* Connect EN to the Photon's D4 pin
* add a 2-conductor connector for the motor, right-angle mounted. maybe the same JST connector used for the battery, though i'm interested in knowing whether there are smaller alternatives.


https://www.precisionmicrodrives.com/tech-blog/2016/05/16/how-drive-vibration-motor-arduino-and-genuino
