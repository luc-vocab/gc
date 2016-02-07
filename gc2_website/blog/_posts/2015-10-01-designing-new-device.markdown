---
layout: post
title: "Designing a new anti-bruxism device"
date: 2015-10-01 00:00:00 +0000
section: blog
thumbnail: http://res.cloudinary.com/photozzap/image/upload/c_fill,h_400,w_400/v1454776145/gc_website_blog/photon2.jpg
---
For two and a half years, my bruxism symptoms were manageable. After I stopped wearing the early grind control device in August/September 2013, my symptoms continued to steadily improve. Not completely gone, but most days I felt completely normal, without headaches or dizziness, and no pain or muscle tension while chewing. The symptoms would occasionally come back sometimes occasionally, for example I would feel tension in the morning for a few hours.

I knew my bruxism was not gone, because of these occasional flare-ups, and because I was told I sometimes grind my teeth at night, sometimes right after falling asleep. I also strongly suspected some medication would significantly worsen my night-time bruxism, particularly fluticasone/salmeterol (brand name: Seretide). I only use this medication a few times a year, for a few days, when my airways are irritated, but every single time, it makes my bruxism kick into full gear. Looking back, it's not a coincidence that the extreme symptoms appeared end of 2010/early 2011, at a time when I was using the asthma inhaler for a good six months stretch.

Most recently, during a weekend trip to Taiwan, I felt the kind of dizziness in the morning that I can only attribute to bruxism. I am almost certain it must have been due to increased teeth grinding the nights before, but I had no way of proving that. An idea started forming in my head to build a new device, comfortable and convenient enough to wear every night, which could alert me to this sort of situation. Taiwan is a huge semiconductor manufacturer and some billboards advertise this in the airport. So my mind was on electronics. 

Besides keeping in touch with the fine folks on the [Bruxhackers Mailing List](https://groups.google.com/forum/#!forum/bruxhackers), I hadn't touched any electronics, hadn't powered up any microcontrollers or held a soldering iron in the last two years. I loaded up [Sparkfun.com](http://www.sparkfun.com) to see what's new and came across this little gem:

<img src="http://res.cloudinary.com/photozzap/image/upload/c_scale,w_1024/v1454776145/gc_website_blog/photon2.jpg" class="img-responsive">

The [Particle Photon](https://www.particle.io/) is an Arduino-compatible, wifi-enabled dev board, made to work seamlessly with the _Particle Cloud_. In short, you can call a REST API to invoke a function on the Photon, or query a particular variable value, without worrying about network connectivity behind a router, or setting up an internet-facing server. Firmware flashing happens over the internet as well. This seemed like a perfect platform to build a bruxism-tracking device on. With the previous grind control device, I always felt like I was running blind. I couldn't see in realtime what the EMG value was, because the only way of outputting data was connecting to the USB/serial port, and that wasn't compatible with the EMG sensor, which had to be electrically isolated.

With the grind control device, at every development cycle, I had to take the microSD card out, copy the data over, process it, and start over. This was extremely time consuming. The new device definitely had to be **Wireless**. Additionally, I had to charge 2x 9V batteries every single day, and replace the old ones. 

Objectives for the new device:

* **Wireless**: for every day usage, it's too much of a hassle to have to disassemble and take the microSD card out.
* **On-board battery charging**: it should charge by plugging in a USB cable, just like every modern gadget.
* **Controllable from a smartphone**: because the device is small, and worn against the user's head, it doesn't have an LCD display. But it should relay information back to a mobile device, smartphone or tablet.
* **Comfortable**: it should be comfortable enough to wear every night. This means the size should be small, and there shouldn't be a lot of wires sticking out.

An essential component in an anti-bruxism device is the EMG ([Electromyography](https://en.wikipedia.org/wiki/Electromyography)) sensor. The grind control device used the [http://www.advancertechnologies.com/](Advancer Technologies) Muscle Sensor V3. It just so happened that they released a new version, the [Myoware Sensor](https://www.kickstarter.com/projects/312488939/myowaretm-harness-the-power-of-your-muscle-signals/description) right around the time I set out to build a new device.

<img src="http://res.cloudinary.com/photozzap/image/upload/c_scale,w_1024/v1454817653/gc_website_blog/P2070010.jpg" class="img-responsive">

The Myoware has a number of advantages:

* **Doesn't require a dual power supply**: can be powered from a +3.3V input
* **Has electrode snaps directly on the PCB**, which makes wires short and as a result is less susceptible to interference.
* **Very small in size**: can be stuck directly on the face.

It seems like a perfect fit for this project. Sparkfun has released a number of shields for the Photon which I can also directly use. Here are the building blocks for the new device:

* **Particle Photon**: microcontroller and WIFI
* **Myoware sensor**: EMG sensor
* **Sparkfun Battery Shield**: LIPO battery charging and battery gauge
* **Sparkfun IMU Shield**: 3-axis accelerometer, gyroscope and magnetometer, to track movements during sleep.

As a first step, a PCB must be designed that regroups all of these elements onto one board. The Myoware sensor will be external, connected using a short cable.

Check back soon for updates !