---
layout: post
title: "Accelerometer-only Bruxism Detection Solution"
section: blog
thumbnail: https://res.cloudinary.com/photozzap/image/upload/c_fill,g_north,h_400,w_400/v1468048450/gc_website_blog/accelerometer/P1050250.jpg
---

When I set out to work on this project, the Myoware muscle sensor was deemed to be an essential component, necessary to sense muscle activity and gauge the level of night-time bruxism activity. My 2013 project also used an EMG sensor, the Muscle Sensor v3. A lot of people who attempt to build bruxism devices (as is the case for a few members on the bruxhackers mailing list) independently come to the same conclusion, that an EMG sensor is ideal for a bruxism device.

Issues with the Myoware muscle sensor
-------------------------------------

And there a lot of things that seemed very positive about the Myoware, which I <a href="/2015/10/01/designing-new-device/">wrote about</a>:

<img src="https://res.cloudinary.com/photozzap/image/upload/c_scale,w_1024/v1454817653/gc_website_blog/P2070010.jpg" class="img-responsive">

 * **Doesn’t require a dual power supply:** can be powered from a +3.3V input
 * **Has electrode snaps directly on the PCB**, which makes wires short and as a result is less susceptible to interference.
 * **Very small in size**: can be stuck directly on the face.

After months of testing though, I've come to the conclusion that the reliability level of the Myoware makes it not a good fit for this project. Among some of the issues I've found:

 * **Very hard to locate proper placement of electrodes**. This was already an issue with the Muscle Sensor v3, but it's somehow worse with the Myoware. This has to do the close placement of the mid-muscle and end-muscle electrodes which are on the Myoware PCB.
 * **Sometimes doesn't pick up any signals at all**: this is by far the biggest issues. There are some nights where I would setup the device before sleep and would spend 20 minutes trying to get it setup and not be successful eventually. Needless to say I don't need this kind of stress right before sleep.
 * **Electrodes stop making contact after some time**: so even if I set it up successfully, I may wake up to find out that the last few hours of recording show a completely flat EMG signal.
 
I made a decision to give up on using an EMG sensor. So without the EMG sensor, how do we detect bruxism ? Using motion sensors.

Accelerometer-only solution
---------------------------

Bruxism involves movement of the lower jaw. In principle, by sensing movement of the chin, we should be able to detect repetitive motion indicating bruxism. Detecting a static clenching position would be more difficult, in the absence of movement.

{% include image.html scaling="c_fill,w_1024" image="accelerometer/P1050254.jpg" %}

I'm currently experimenting with a device with two sets of accerometers:

* **A BNO055 integrated with the main PCB, attached to the headband.** This is a really advanced chip with 3 axis accelerometer, gyroscope, and magnetometer, capable of sensor fusion to calculate absolute orientation. The accelerometer uses a 14bit ADC and the gyroscope is 16bit, so this chip at least on paper seems more precise than others.
* **A MMA8452 fixed on the user's chin.**. This is a simple 3-axis accelerometer with 12bit ADC.

The way I currently place them, their axes are not completely aligned, so I can't directly compare their output. They are also different chips which will give slightly different readings. However in the future I'm planning on having the two IMUs be exactly on the same plane, with one on the forehead and one on the chin.

{% include image.html scaling="c_fill,w_1024" image="accelerometer/P1050250.jpg" %}

Eventually I'd like to use two BNO055's, as they are more functional and more precise. The gyroscope in particular, is remarkably low-noise. The gyro values read very very close to zero in absence of movement. 

Sensing motion with an IMU
--------------------------

Sensing motion with an IMU (Inertial Measurement Unit) can be done in a number of ways. The easiest way of doing so is using an accelerometer, which senses acceleration, the first derivative of motion. In practice, it means I should see a reading change if my chin moves left to right.

In practice, most accelerometer give you not only linear acceleration, but also gravity information. So if I'm lying on the side, and reading from the accelerometer's X axis, the values read will be different than if I were lying on my back, with the accelerometer's X axis perpendicular to the gravity vector.

With accelerometers, it should be expected to accomodate for varying base levels based on the sleeping positions. The BNO055 has an interesting solution to this problem and can output something called a **linear gravity vector**. Using its sensor fusion algorithms, the chip will attempt to determine where the gravity vector is, and output only what it thinks is true movement. This is an interesting feature that seems to work quite well, provided the BNO055 is well calibrated.

However i'm currently not using the BNO055 on the chin sensor, but the MMA8452, which is a simpler accelerometer, not capable of outputting linear acceleration. That's OK, I can still work with the signal which includes gravity. I've found that calculating a running standard deviation over the last 20 samples or last 2 seconds gives me a good idea about true movement, and is easy to spot in a whole night's chart.

{% include image.html scaling="c_fill,w_1024" image="accelerometer/accel_bruxism_movements.png" %}
*Above: potential bruxism movements. The accelerometer registers movement on X and Y axes. The headband IMU (not shown) registers very little movement which indicates jaw-only movement*

Another way of detecting motion is using a gyroscope. This is a sensor which detects change in angular velocity, but i've found it's quite good at detecting movement. In theory, a perfectly linear translation movement would be imperceptible by a gyroscope, but in practice, the 16bit gyroscope within the BNO055 detects movement quite nicely. It is able to detect the slight jaw movement of bruxism even though it is currently placed near my temple. I expect that once the chin sensor with BNO055 is ready, it'll show a very clear signal in case of lower jaw movement.

Once I have the exact same chip on the forehead and chin, aligned along the same axis, it'll become extremely easy to determine whether the jaw is moving independently of the head.

Lastly, one of the most powerful feature of the BNO055 is absolute orientation tracking. It can output roll, pitch and heading angles (or equivalent using quarternions) which indicate the user's head's absolute orientation in space. This will allow us to track sleeping positions.

I'm planning on experimenting further with my prototype, and am currently building, with the help of my PCB designer, a smaller version of sleeptrack using two external BNO055-based IMUs. So far I'm feeling very positive that this could be a very functional bruxism tracking solution, though some work is left to be done to prove this.