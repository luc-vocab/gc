Grind Control Device
====================

This device attemps to prevent night-time bruxism. It works by detecting muscle activation on the temporalis 
muscle and will emit an audio signal to wake the user up if bruxism is detected.

Features:
* Has an audio jack to plug-in headphones. These headphones are use to play an audio tone that will wake up the user during sleep if grinding is detected.
* An [EMG circuit](http://www.advancertechnologies.com/) is used along with custom-made electrodes that are placed on the temporalis muscle.
* A micro-SD card is used to record bruxism patterns for later analysis.
* A custom-made case was designed which can be 3d-printed.

The design is heavily inspired by the commercially available [Grind Care](http://grindcare.com/) device (which appears to no longer be available), except an audio signal is used instead of electrical impulses.

Photos
======

The device is worn around the neck, held by a strap:
![Device](https://raw.github.com/lucwastiaux/gc/master/photos/version1/20130601-P6010001.jpg)

Three electrodes are stuck on the temporalis muscle, the red one on the middle of the muscle, the blue one at the end, and the black one at a neutral area of the skin. Earphones are used for the audio signal:
![Electrodes](https://raw.github.com/lucwastiaux/gc/master/photos/version1/20130601-P6010010.jpg)

The device with strap, earphone, and electrode cable. The case is a custom designed enclosure, designed in Sketchup and printed using Selective Laser Sintering:
![Device 2](https://raw.github.com/lucwastiaux/gc/master/photos/version1/20130601-P6010011.jpg)

Inside of the device, with two 9V batteries:
![Inside](https://raw.github.com/lucwastiaux/gc/master/photos/version1/20130601-P6010012.jpg)

The printed circuit board. You can see the Arduino Micro microcontroller, micro-sd card reader, muscle sensor, switches and 3.5mm headphone jack.
![PCB](https://raw.github.com/lucwastiaux/gc/master/photos/version1/20130601-P6010019.jpg)

The printed circuit board, designed in Eagle:
![PCB](https://raw.github.com/lucwastiaux/gc/master/photos/version1/eagle_board.png)

Schematics in Eagle:
![PCB](https://raw.github.com/lucwastiaux/gc/master/photos/version1/eagle_schematic.png)



