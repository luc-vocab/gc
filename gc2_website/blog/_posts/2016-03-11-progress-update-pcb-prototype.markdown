---
layout: post
title: "Circuit Board Manufacturing and Prototype Updates"
date: 2016-03-11 00:00:00 +0000
section: blog
thumbnail: https://res.cloudinary.com/photozzap/image/upload/c_fill,g_center,h_400,w_400/v1456221905/gc_website_blog/progress_update_1/sleeptrack_v2_pcb_front.jpg
---

Manufacturing of the revision 2 of the sleeptrack protype is almost complete and I'm waiting for the board to be shipped from Shenzhen, China. While the manufacturing went further this time, there are a number of issues which will have to be fixed before this version is fully usable. Additionally, i've been experimenting with a prototype and ran into issues with the EMG sensor.

{% include image.html scaling="c_scale,w_1024" image="progress_update_1/sleeptrack_v2_pcb_front.jpg" %}

Above is the top of the rev2 of the Sleeptrack PCB (Printed Circuit Board). The dimensions are about **2"x2" (5.1cm x 5.1cm)**. You can distinguish the following components:

* [Particle Photon P1](https://docs.particle.io/datasheets/p1-datasheet/) (flat component on the bottom half). This is the CPU and wireless adapter of the device.
* **Buzzer** on the lower right. Can emit sounds to inform the user about various events.
* **3.5mm audio jack** on the upper right. Headphones can be plugged into this connector for the audio biofeedback signal.
* **JST battery connector**, **On/Off switch**, **EMG connector** at the top.
* **Micro USB connector** and **Push buttons** on the left.

{% include image.html scaling="c_scale,w_1024" image="progress_update_1/sleeptrack_v2_pcb_back.jpg" %}

One might say it looks fairly complete, however the **IMU** footprint is wrong and as such that chip couldn't be soldered on (lower right above). Without this, the device cannot detect movement or head position. The board house also indicated there were some issues with the USB connector although i'm not clear what the problem is yet. I have yet to receive this board, the above pictures are from the manufacturer. Even though there are issues with this revision, I expect it can still be used to test and make further progress on both the hardware and software front.

For future revisions, I'm considering using a [BNO-055](https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/overview) chip. It's a 9-DOF IMU sensor with built-in sensor fusion using an ARM Cortex-0. I didn't realize this a month ago, but integrating sensor data from an accelerometer, gyroscope and magnetometer is that that easy and requires a lot of CPU power to get a stable reading. The above chip does it all internally and outputs an absolute orientation. This could be used for example to track head position throughout the night. 

{% include image.html scaling="c_scale,w_1024" image="progress_update_1/sleeptrack_prototype_top.jpg" %}

While waiting for the circuit board to be manufactured, I've been experimenting with a *perma-proto* version of the device. This allows me to solder all the components together without loose wires coming out. I've then mounted this prototype on a custom-made laser-cut acrylic plate as shown below.

{% include image.html scaling="c_scale,w_1024" image="progress_update_1/sleeptrack_prototype_side.jpg" %}

This allows me to wear the device on the head using a headband, in a similar fashion to the way the final product will be worn. Obviously the current size is much bigger than the final prototype (about 2-3 times as big).

{% include image.html scaling="c_scale,w_1024" image="progress_update_1/sleeptrack_prototype_head.jpg" %}

The photo above shows the full setup. As mentioned it's quite bulky. The rectangular board will shrink down in size considerably. However the smaller **EMG / muscle sensor** will remain that way. During experimentation, I have run into issues with this sensor. The first time I placed it on the temporal muscle, it would show normal readings, low when the muscle is at rest, then elevated when contracting the muscle. However after slightly readjusting / touching the sensor, occasionally readings become inaccurate. My suspicion is that slightly readjusting the sensor causes the electrode contact to become bad and as a result the EMG signal becomes unreliable. 

I had already noticed placing the sensor was challenging, more so than placing individual electrode cables on the [Grind Control device](/2013/06/01/experiences-with-grind-control-device/). Early on I noticed that if the cable connected to the sensor had any kind of rigidity or weight, there would be an uncomfortable pulling sensation on the skin. This prompted me to build a cable with extremely flexible silicone wires, after which the issue went away. However, placing the sensor exactly right is difficult. I suspect the EMG sensor is too short, as a result the mid-muscle electrode and the begin muscle electrode are not exactly where they should be.

For future revisions of the device, I am thinking of using the EMG sensor's electrode connector and just connect a cable to that, so that the three electrodes can be reliably placed and reduce the risk of accidentally moving them. The EMG sensor board would be placed on top of the main device, with electrodes wires coming out.

Despite these issues, it feels like progress is being made, slowly !