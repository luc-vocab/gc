2015/12/10
* buzzer: can the smaller magnetic buzzers work ?
* PCB color: red
* url, revision number and names on the silkscreen
* can buttons be of different color ? easier for the user.

2015/10/22
* low dropout supply vs. SMPS ?
 * Madison suggests going with the low dropout supply as it's the reference implementation
* The buzzer and headphone outputs should be on separate pins, they will be activated separately
* On the slide switch, add a resistor between the 3.3V and the slide switch pin to limit current
* luc to send over datasheet for connector
  * http://www.mouser.hk/Search/ProductDetail.aspx?R=61900311021virtualkey51110000virtualkey710-61900311021 . 3 pins with 2.54mm pitch
* EMG connector should be at the bottom of the board as it'll avoid creating a right angle when going to the EMG sensor. The connector sticks out quite a bit with the horizontal male pins, I think it's fine to have it stick out, as the cable will be inserted/removed daily, and the user should be able to grip the cable assembly.

2015/10/14
* molex connector for EMG sensor: the one on the kickstarter looks different (http://imgur.com/a/07ak6)
   * Luc to give more details after receiving muscle sensor and cables 
* 2 pos slide switch (SS12SDH2) on the arduino prototype, it felt like input and output were briefly shorted
  * in my previous device, I had a resistor between one of the poles of the switch and the arduino 5V pin https://raw.githubusercontent.com/lucwastiaux/gc/master/photos/version1/eagle_schematic.png
* SPDT push button: can't tell from the image what the orientation will be ? should be same as 2 pos slide switch
  * cleared up from the call, it is a right-angle mounted switch as well
* on/off switch: yes I need that, to open close the battery circuit
* the MAX5160L circuit you show seems complicated, it looked like there were other choices for mono (example 1 here seems to be good ? https://www.maximintegrated.com/en/app-notes/index.mvp/id/1828)
* JTAG: not sure, however the push buttons and RGB LED seem like a must
  * please add the JTAG pins as unused through-holes
* go over P1 integration page: https://docs.particle.io/guide/how-to-build-a-product/pcb-design/
  * please add the RGB LED and the 2 push buttons as described here https://docs.particle.io/datasheets/photon-datasheet/#schematic-user-i-o
* any choices regarding the regulator ? Or 3.3V regulators are quite standard ?
  * cleared up during the call, you will use a part you're familiar with
