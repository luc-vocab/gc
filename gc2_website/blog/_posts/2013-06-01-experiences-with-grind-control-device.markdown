---
layout: post
title: "Experiences Using the Grind Control Device"
date:   2013-06-01 12:45:00 +0000
section: blog
---
For many months I had grown convinced that the source for my issues is an unconscious night-time behavior of my jaw that no one could explain. It seemed obvious to me that if I could detect this and stop it then I would solve my problem. To me this has been a very successful project. Since I started using the device in April 2013, my symptoms have improved progressively but very noticeably.

The first times I got woken up by the beeping audio sound were a victory for me as I had finally found an effective system to put a stop to this destructive unconscious habit. I distintively remember being woken up a few times with my jaw clenched and the device beeping into my ears. I had no idea why my jaw was clenched but only after I became more conscious would I relax my jaw muscles. This was a revelation and yet another proof that my problems were due to night-time, unconscious bruxism.

There are some drawbacks, the biggest one being interference with your sleep. In my case, it was a lot better to interrupt my sleep than to let the destructive night-time bruxism keep on happening. However sleep segmentation may have some negative side effects. Ironically, sleep deprivation may lead to headaches so you need to be careful if you try something like that.

One source of frustration is that bruxism is not well understood by the medical community. Most doctors and dentist will not be able to help you much besides suggesting mouth guards and other dental treatments. There is a belief that bruxism is caused by malocclusion or other problems in the mouth. While that may be true, i've also heard dentists say that you could have perfect teeth and still experience bruxism symptoms. Other people will say it's due to stress. In my personal opinion, bruxism becomes a habit that is very hard to get rid of. While it may initially be due to stress or dental problems, it will become a habit that you cannot control.

I now no longer wear the device regularly as my symptoms have improved significantly. I will occasionally wear it if I feel symptoms coming back. But judging by the last 2 months, it seems like it is possible to have a lasting improvement.

Problems with current design
----------------------------

 *  No LCD screen. This makes it difficult to experiment with different algorithms as you can't tell what the current reading is.
 * Only works off 9V batteries. I end up having to charge a new set every day.
 * The sensitivity is based on the EMG readings from my muscles. It may vary from one person to another. Right now the only way to change this is in the code.

Potential improvements
----------------------

 * Add small LCD screen to show current readings and accumulated high-readings. This will be useful if the device starts beeping in the middle of the night and you don't know why.
 * Add some buttons to configure sensitivity. This would allow a new user to use the device without programming.
 * As an alternative to the above, I could use a Low-Energy Bluetooth chip and control the whole thing from an iPhone or Android phone. I have no experience with that, but it's an interesting idea. Not sure what it would mean for power consumption, although with on-board storage, I wouldn't need to keep the bluetooth radio on the whole night, only when the user is configuring the device.
 * Run off a LIPO battery. This presents some challenges, as the EMG sensor requires a dual power supply, and with the 2 9V batteries forming a +/- 9V dual supply, the output voltage of the EMG sensor moves between 0.1V and 0.9V. This is only a small range of the Arduino's 0V-5V resolution. Maybe my mucles are weak. If I were to use a LIPO battery, I would have to first use a step-up converter to increase the voltage from 3.7V to 9V, and then create a dual power supply. This is beyond my level of knowledge. There are some ready-made boost converters you can buy, I may look into those. Then I may need a charging circuit for the LIPO, but again that's beyond my area of expertise.
