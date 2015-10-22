2015/10/22
* low dropout supply vs. SMPS ?
* payment
* audio circuit uses different parts, OK
  * green LED ?
* 

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
