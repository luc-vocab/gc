Project Overview
================
The project consists of building a device to monitor and log night-time bruxism as well as associated sleep data. More background here: https://github.com/lucwastiaux/gc/edit/master/README.md

Board Components
================
* Particle Photon board https://www.particle.io/. There seem to be two options, either include the SMT (headerless) version of the Photon dev board, or use the P1 chip, which requires a voltage regulator, some buttons and LEDs. 
* All components of this battery shield: https://www.sparkfun.com/products/13626. 
  * I don't need the bottom pads for the barrel connector.
  * I don't need the Alert Interrupt Jumper (https://learn.sparkfun.com/tutorials/photon-battery-shield-hookup-guide?_ga=1.30882105.781200154.1443956391) 
  * I am not clear in what position the I2C pull up jumper needs to be (will try to clarify)
  * If you are using the Photon P1 chip, then we need an additional micro-USB port for charging.
* All components of this IMU shield: https://www.sparkfun.com/products/13629
* A 3-pole connector for connecting to the EMG sensor https://www.kickstarter.com/projects/312488939/myowaretm-harness-the-power-of-your-muscle-signals
  * The 3 poles are Ground, +3.3V regulated, and signal from the EMG
  * The signal from the EMG will be read by an analog input pin on the Photon using analogRead()
* A switch to close/open the battery circuit and turn the device off.
* A side-mounted 2-position switch which sets one of the digital input pins on the Photon to either high or low
  * This will be used to configure an "offline mode" in the software as the Photon expects full time internet connectivity by default
  * The value of the switch will be read using digitalRead()
* 2 side-mounted push-buttons (not sure i'm using the right word) which will be connected to 2 respective digital pins on the Photon.
  * These will be used to interact with the software.
  * The wiring will be similar to this https://www.arduino.cc/en/Tutorial/Button
  * The value of the switch will be read using digitalRead()
* An audio circuit with a 3.5mm jack
  * This will be used to output audio tones using the tone() function.
  * I would like the tone volume to be software-configurable, however I am not clear what the exact circuit should be.
  
