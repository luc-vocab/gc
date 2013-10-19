Grind Control Device
====================

<b>Disclaimer: use this design, or any of the parts at your own risk.</b>

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

History and development
=======================

Symptoms and Diagnosis
----------------------
In October 2010, I started experiencing an unexplained permanent dizziness. It would come and go and be more severe 
during certain weeks. By March 2011, the symptoms had become more severe and included permanent headaches and facial
muscle pain. I started making doctor's appointments, saw three neurologists, one ENT doctor, took blood samples, had an
MRI brain scan but no diagnosis yet. Since the beginning I had been researching an explanation for those symptoms on the
web. Many leads did not pan out but I read that night-time bruxism (unconscious grinding of teeth at night-time) could
be a possible explanations. Several things pointed in that direction:
* I had been told before that I grind my teeth at night.
* My symptoms started one month after the end of an orthodontic treatment. After my orthodontic appliance was removed, I had to wear an Invisalign appliance, which is a transparent plastic mold designed to keep your teeth in place.
* Very quickly after starting to wear the Invisalign appliance, it started to show wear marks at the points of contact of my teeth.

I made an appointment with a New York dentist who made a special mouth guard designed to fit on the front teeth only.
This had a hugely positive impact on my symptoms and two months after wearing that, I believed the problem was gone.

However I occasionally still felt dizziness. In October of 2011, I had ocasional dizziness and semi-permanent headaches again.
This time, I experienced pain when chewing which clearly pointed to an issue with the jaw and especially jaw muscles.
In march 2012, I went to see a dentist in Hong Kong who made another type of mouthguard, and eventually put tooth fillings on my back teeth
to attempt to remediate a malocclusion problem. This had a beneficial impact. The dentist mentioned that night-time
bruxism is extremely challenging to control, that it may be caused by stress, but may become habitual and persist even
after the stress stimulus is removed.

In September 2012, I purchased the Grind Care device which detects night-time bruxism and attemps to condition you out of 
it by triggering a small electric shock. It's a very well made very compact device which you wear at night around your neck.
At electrode is stuck on your face near the temporal muscle. The electrode serves both to monitor muscle activity and
send a shock if muscle contraction is detected.

The grindcare device was the thing that helped me the most up to that point. It would give me some metrics of how many times
I grinded my teeth at night. It started out at 140/night, then progressively moved to 30/night. My symptoms improved radically.
However by December 2012 the number of grinds/night started increasing, as if my body had somehow adjusted and no longer cared
about the electric shock.

The idea started forming in my head that the device should not just send an electric shock, but actually wake me up if 
bruxism is detected.

Development of Grind Control Device
-----------------------------------

In December 2012, I purchased a book on electronics to refresh my knowledge on the topic and learn some of the necessary
techniques such as soldering. I experimented with an USB voltmeter capable of detecting +/- 200mV. I started experimenting
with Operational Amplifiers. However in order to accurately detect muscle activity, you need a true EMG signal processing circuit.
I purchased an Olimex EMG and Arduino kit. That kit turned out not to be sensitive enough to detect temporal muscle activity.
A couple of weeks later, I received my Advancer Technologies Muscle Sensor v3. Through playing around with the electrodes
connected on my face, and measuring output voltage with a multimeter, I realized this was the way to go. The output voltage would
show 0.1V with muscles relaxed, and +0.9V with muscles contracted. This was perfect and exactly within the range that I needed
to process the signal with my Arduino Micro.

I needed some other parts in addition to the EMG sensor and Arduino Micro. In order to study grinding patterns, I needed
to record data and hence got the Adafruit microSD adapter. The programming is quite simple if you follow instructions. I also
needed an audio output. The arduino can output a square wave using the PWM output which is loud enough to wake you up. Using
a trimmer/potentiometer, I can control the volume. I needed a 3.5mm audio jack, some switches to turn the device on/off, and
one "control" switch.

My first prototype was made using an Adafruit perma-proto board. It's a PCB with a breadboard-like layout. I moved my parts
on there and connected them using solid core wire, soldered the whole thing together and cut off part of the board with shears.

I bought an enclosure which was the right size and had 2 9V battery compartments. The whole thing was quite bulky but it was ready
to use. I could then start working on the detection algorithm.

Later, I learned how to design PCBs in eagle and designed the layout you see above. It took me a couple of tries before getting
it right. Once I had the PCB and all components soldered on, I learned 3d modelling with Sketchup. Designed a case that
would fit 2 9V batteries, the PCB, and would have openings for the switches and connectors. That was a surprisingly easy
part of the process. The case is 3d-printed using Selective Laser Sintering.

Algorithm
---------

Once I had the final prototype ready, I started on fine-tuning my algorithm. I analyzed the data on the micro-sd card using
python scripts. Every morning, i would wake up and try to detect grinding patterns. The baseline output value from the EMG sensor
would be around 30 in general with no activity. When contracting the muscle, it would go up to 150-200. Sometimes it would stay 
at those high values for minutes during the night, about 1-2 hours after falling asleep. Detecting this high activity was easy,
but there were also times where I would see high values, low values alternating but it clearly didn't look like my jaw was 
at rest during that time. The tradeoffs were, I could set up the algorithm to be very sensitive, but there was a higher
risk of being woken up on a false alarm. Or I could set it up to be more permissive, and I would be unable to detect
certain grinding patterns and not fully solve the problem.

I experimented with many algoritms and ended up settling on a relatively easy one: count the number of "high"
values over the last 10 seconds (sampling 4 times per second). If they exceed 5, go into "alert 1" state, and issue an audio
beep with every new high value. Once the number of high readings exceed 10, issue a higher pitched beep with every new high value.
If they exceed 15, then go into alarm mode and continuously beep until the user resets the device.

Notes about using Arduino Micro
-------------------------------

Working with the Arduino Micro was a good experience overall. The only issue is the limited amount of RAM: 2.5kb. This
is very restrictive. The most annoying issue is that the device will automatically reset if you run out of RAM. It took
me a long time to understand that. Other than that, the Arduino is perfect for this application. The power consumption is
such that I'll go through 2 500mah 9V batteries in one night. Not a big deal since I can recharge them easily.

Experiences using the device
============================

For many months I had grown convinced that the source fo my issues is an unconscious night-time behavior of my jaw that 
no one could explain. It seemed obvious to me that if I could detect this and stop it then I would solve my problem.
To me this has been a very successful project. Since I started using the device in April 2013, my symptoms have improved
progressively but very noticeably.

There are some drawbacks, the biggest one being interference with your sleep. In my case, it was a lot better to interrupt
my sleep than to let the destructive night-time bruxism keep on happening. However sleep segmentation may have some negative side
effects. Ironically, sleep deprivation may lead to headaches so you need to be careful if you try something like that.

One source of frustration is that bruxism is not well understood by the medical community. Most doctors and dentist will
not be able to help you much besides suggesting mouth guards and other dental treatments. There is a belief that bruxism
is caused by malocclusion or other problems in the mouth. While that may be true, i've also heard dentists say that you could
have perfect teeth and still experience bruxism symptoms. Other people will say it's due to stress. In my personal opinion,
bruxism becomes a habit that is very hard to get rid of. While it may initially be due to stress or dental problems, it will
become a habit that you cannot control.

I now no longer wear the device regularly as my symptoms have improved significantly. I will occasionally wear it if I feel
symptoms coming back. But judging by the last 2 months, it seems like it is possible to have a lasting improvement.

Problems with current design
----------------------------
* No LCD screen. This makes it difficult to experiment with different algorithms as you can't tell what the current reading is.
* Only works off 9V batteries. I end up having to charge a new set every day.
* The sensitivity is based on the EMG readings from my muscles. It may vary from one person to another. Right now the only way to change this is in the code.

Potential improvements
----------------------
* Add small LCD screen to show current readings and accumulated high-readings. This will be useful if the device starts beeping in the middle of the night and you don't know why.
* Add some buttons to configure sensitivity. This would allow a new user to use the device without programming.
* As an alternative to the above, I could use a Low-Energy Bluetooth chip and control the whole thing from an iPhone or Android phone. I have
 no experience with that, but it's an interesting idea. Not sure what it would mean for power consumption, although with on-board storage, I wouldn't need
to keep the bluetooth radio on the whole night, only when the user is configuring the device.
* Run off a LIPO battery. This presents some challenges, as the EMG sensor requires a dual power supply, and with the 2 9V batteries forming a +/- 9V dual supply, the output voltage 
of the EMG sensor moves between 0.1V and 0.9V. This is only a small range of the Arduino's 0V-5V resolution. Maybe my mucles are weak. If I were to use a LIPO batterie, I would
have to first use a step-up converter to increase the voltage from 3.7V to 9V, and then create a dual power supply. This is beyond my level of knowledge. There are some ready-made boost
converters you can buy, I may look into those. Then I may need a charging circuit for the LIPO, but again that's beyond my area of expertise.


Interested in using this ?
==========================

In its current state, I cannot recommend using this device to anyone who is not an engineer. You may have to do a lot of 
experimentation to get it work for you, however if your bruxism symptoms are as severe as mine were, I can only encourage 
you to pursue this. I'm not planning on making an updated version right now, but several people have expressed interest.
If you have any ideas around this please contact me.
