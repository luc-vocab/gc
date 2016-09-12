2016/02/08 Checklist for Rev2
=============================

Changes to the board
------------------
* Fix speaker footprint, was too large
* RGB lead position, was too close to the photon
* Mechanical mounting pads too large and covered with solder mask for the USB connector (I don't understand the issue but it's in your email)
* Solder mask covering mechanical pad for the audio amp U9
* Correct placement of push buttons so that they're facing outward
* JST and EMG connector should be 90-degree angled connectors (will provide exact part numbers if you can't find them)
* Add an unsoldered footprint for a potential I2C extension board. Part for the footprint: http://www.digikey.com/product-search/en?mpart=61900411021&v=732 . You can order the pins below in way that makes sense to reduce the complexity of the board.
  * GND
  * +3.3V
  * D0 (SDA)
  * D1 (SCL)
* Remove mode switch to make space for the above connector holes.

General checks
--------------
* Make necessary changes to PCB and update artwork.
* 1:1 Print out to test component fitment, silkscreen, REF DES, etc...
* Validate Gerber files using online Gerber viewer. (http://www.gerber-viewer.com/)
* Ask for proofs before manufacture to confirm we are receiving the expected results
* Proof read BOM and provide enhanced documentation to assembly team. (Reference designation column for BOM components, # parts, # of individual parts, assembly drawing, etc..)
* Preform QC and Electrical Check of assembled boards.


2015/11/05 Review of annotated PDFs generated on nov 1st
========================================================

Audio
-----
The buzzer circuit looks good
* I can see it's the recommended operating circuit from the SDR08540M3-01 datasheet.
* A4 pin is the correct one and it's capable of PWM output https://docs.particle.io/reference/firmware/photon/#tone-

Headphone circuit
* I like that you picked the SJ1-3515N 3.5mm jack, it has a low profile so less thickness
* my understanding is pin 4 will be kept high when no plug is inserted and will become low once pins 4 and 2 become disconnected

AD5220 circuit
* the datasheet (http://www.analog.com/media/en/technical-documentation/data-sheets/AD5220.pdf) doesn't indicate audio as a potential application. Any concerns ?
* Should D5 / CLK be held high ? I'm worried about what will happen when I power-on the device and the D5/D6 outputs are floating temporarily (until I set their outputs in software). The datasheet says the potentiometer starts up centered, so I'll how many times to adjust down or up before outputting sound, but that logic may not work if there has been some inadvertent increases/decreases ?
* Which version of the AD5220 will you pick, 10k, 50k, 100k ?

TPA2005D1DRB
* is there a difference between the Sleeve output (VOUT-) and GND ? I see on the SJ1-3515N, it seems Sleeve and GND are one and the same somehow.
* I don't understand the point of the shutdown circuit and associated LED ? I couldn't understand from the datasheet what that shutdown pin is for.
* besides those two things, it looks good to me.


Connectors / Push Buttons
-------------------------

* Auxillary slide switch: can you put a resistor between the 3V3 and the D2 pin
* Auxillary switches 1 and 2: what is VIN ? Should that be 3V3 instead ? Also, is it advisable to put a resistor between the 3V3 and the photon pin ? I think you mentioned just to be safe we can add that ?
* I think you may have slightly changed the pin connections between the mode slide switch and push buttons (compared to https://github.com/lucwastiaux/gc/blob/dev/gc2_specs/SPECS.md#proposed-pin-layout), but let's leave it like that, as D2, D3, D4 will all be setup as digital inputs and are interchangeable.


IMU
---
* you can remove all of the jumpers as long as both SDA/D0 SCL/D1 each have their pull-up resistors I think.


Power circuit
-------------
MAX17043
* correct me if i'm wrong, the wiring doesn't seem to match the sparkfun schematics (https://cdn.sparkfun.com/datasheets/IoT/Photon_Battery_Shield_v10.pdf) CELL should go to VBAT, it seems you have it connected to GND. Your previous schematic looked more correct. 

* I see the regulator circuit is fully matching the reference P1 schematic now.


Photon P1
---------

Not reviewing as I think you only removed the JTAG connectors.
