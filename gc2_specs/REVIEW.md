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
* 
