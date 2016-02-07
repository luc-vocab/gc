---
layout: post
title: "Development of Grind Control Device"
date:   2013-04-01 12:45:00 +0000
section: blog
thumbnail: https://res.cloudinary.com/photozzap/image/upload/c_fill,h_400,w_400/v1454745966/gc_website_blog/20130601-P6010019.jpg
---
In December 2012, I purchased a book on electronics to refresh my knowledge on the topic and learn some of the necessary techniques such as soldering. I experimented with an USB voltmeter capable of detecting +/- 200mV. I started experimenting with Operational Amplifiers. However in order to accurately detect muscle activity, you need a true EMG signal processing circuit. I purchased an Olimex EMG and Arduino kit. That kit turned out not to be sensitive enough to detect temporal muscle activity. A couple of weeks later, I received my Advancer Technologies Muscle Sensor v3. Through playing around with the electrodes connected on my face, and measuring output voltage with a multimeter, I realized this was the way to go. The output voltage would show 0.1V with muscles relaxed, and +0.9V with muscles contracted. This was perfect and exactly within the range that I needed to process the signal with my Arduino Micro.

I needed some other parts in addition to the EMG sensor and Arduino Micro. In order to study grinding patterns, I needed to record data and hence got the Adafruit microSD adapter. The programming is quite simple if you follow instructions. I also needed an audio output. The arduino can output a square wave using the PWM output which is loud enough to wake you up. Using a trimmer/potentiometer, I can control the volume. I needed a 3.5mm audio jack, some switches to turn the device on/off, and one "control" switch.

<img src="https://res.cloudinary.com/photozzap/image/upload/c_scale,w_1024/v1454745949/gc_website_blog/photo1.jpg" class="img-responsive">

My first prototype was made using an Adafruit perma-proto board. It's a PCB with a breadboard-like layout. I moved my parts on there and connected them using solid core wire, soldered the whole thing together and cut off part of the board with shears.

I bought an enclosure which was the right size and had 2 9V battery compartments. The whole thing was quite bulky but it was ready to use. I could then start working on the detection algorithm.

Later, I learned how to design PCBs in eagle and designed the layout you see above. It took me a couple of tries before getting it right. Once I had the PCB and all components soldered on, I learned 3d modelling with Sketchup. Designed a case that would fit 2 9V batteries, the PCB, and would have openings for the switches and connectors. That was a surprisingly easy part of the process. The case is 3d-printed using Selective Laser Sintering.

<img src="https://res.cloudinary.com/photozzap/image/upload/c_scale,w_1024/v1454745966/gc_website_blog/20130601-P6010019.jpg" class="img-responsive">
<img src="https://res.cloudinary.com/photozzap/image/upload/c_scale,w_1024/v1454745965/gc_website_blog/20130601-P6010011.jpg" class="img-responsive">

Algorithm
---------

Once I had the final prototype ready, I started on fine-tuning my algorithm. I analyzed the data on the micro-sd card using python scripts. Every morning, i would wake up and try to detect grinding patterns. The baseline output value from the EMG sensor would be around 30 in general with no activity. When contracting the muscle, it would go up to 150-200. Sometimes it would stay at those high values for minutes during the night, about 1-2 hours after falling asleep. Detecting this high activity was easy, but there were also times where I would see high values, low values alternating but it clearly didn't look like my jaw was at rest during that time. The tradeoffs were, I could set up the algorithm to be very sensitive, but there was a higher risk of being woken up on a false alarm. Or I could set it up to be more permissive, and I would be unable to detect certain grinding patterns and not fully solve the problem.

I experimented with many algoritms and ended up settling on a relatively easy one: count the number of "high" values over the last 10 seconds (sampling 4 times per second). If they exceed 5, go into "alert 1" state, and issue an audio beep with every new high value. Once the number of high readings exceed 10, issue a higher pitched beep with every new high value. If they exceed 15, then go into alarm mode and continuously beep until the user resets the device.

Notes about using Arduino Micro
-------------------------------

Working with the Arduino Micro was a good experience overall. The only issue is the limited amount of RAM: 2.5kb. This is very restrictive. The most annoying issue is that the device will automatically reset if you run out of RAM. It took me a long time to understand that. Other than that, the Arduino is perfect for this application. The power consumption is such that I'll go through 2 500mah 9V batteries in one night. Not a big deal since I can recharge them easily.