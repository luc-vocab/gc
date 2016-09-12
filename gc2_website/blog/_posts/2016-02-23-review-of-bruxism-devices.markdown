---
layout: post
title: "Review of Bruxism Devices on the market"
date: 2016-02-23 00:00:00 +0000
section: blog
thumbnail: https://res.cloudinary.com/photozzap/image/upload/c_fill,g_north_west,h_400,w_400/v1456221905/gc_website_blog/bruxism_devices/grindcare_v2.jpg
---

There are a number of devices that help monitor or attempt to prevent bruxism on the market. Most of them use an [EMG](https://en.wikipedia.org/wiki/Electromyography) sensor to detect muscle activation, either on the temporal muscle or the masseter muscle. Some have biofeedback functionality while others are purely allow diagnosing of bruxism / teeth grinding. Some are generally available to the public while others are sold to dentists only. Here is a list of the devices that I know about. If you know of a device which we didn't list, please email us: {% include email.html %}

<p style="margin-bottom: 50px;"></p>

Readily available devices
=========================

These devices should be readily available to purchase now.

Grindcare v1
------------

{% include image.html scaling="c_scale,w_1024" image="bruxism_devices/grindcare.jpg" %}

I purchased a Grindcare in September 2012 after coming across their website. Although the device is marketed towards dental professionals, I was able to get in touch with one of their salespeople and buy the device during a dental conference. The device consists of a base station with an LCD screen and buttons, and a small wearable unit worn at night, connected to the electrode. The wearable unit is inserted into the base station for charging and configuration, and worn around the neck at night.

The first night, the device is configured in monitoring mode only, and subsequent nights, the biofeedback effect is turned on. The feedback consists of a small electric shock which is supposed to unconsciously discourage the clenching action of the temporal muscle. The strength of the effect is configurable, and ranges from barely perceptible, to a electric stimulus so strong that it causes the eyelid to involuntarily close. The electric stimulus is delivered through the same electrode that measures muscle activity.

During the first three months of use, the Grindcare had a tremendous positive effect on my night-time bruxism. The device would provide a *grind count* at the end the night. When I first started wearing the device, I would average 140 grinds / night. This progressively moved down to 10/night. After 3 months however, my bruxism reappeared and the grind count moved back up to 100, 200 and even 300 / night. This was with the electric stimulus at the highest setting. It was as if my body had learned to ignore the electric shock and the impulse to clench my teeth was stronger than anything else. One of the frustrating aspects is that I didn't have detailed data about when the bruxism activity was happening, as the device would just give me a total *grind count* at the end of the night. I wished for more detailed data to answer questions such as:

* When is the bruxism happening.
* Does the electric stimulus actually hinder the bruxism activity.

Overall the Grindcare device was of a lot of help, but in my case, I need a much stronger anti-bruxism signal to prevent night-time clenching. The Grindcare device is what gave me the idea to look into electronic bruxism detection which eventually led me to build the [Grind Control device](https://github.com/lucwastiaux/gc). For a while, even considered building a device around it, by detecting the electric shock sent to the electrode and leveraging the Grindcare's clenching detection mechanism.

There are number of [Youtube Videos](https://www.youtube.com/user/GrindCare) demonstrating the Grindcare device in action. It's worth noting that the company originally behind Grindcare has been sold and a new version of the device is in development (see below).
 
Sleepguard
----------

{% include image.html scaling="c_scale,w_1024" image="bruxism_devices/sleepguard_biofeedback.jpg" %}

The [Sleepguard](http://mysleepguard.com/) is a headband which uses a couple of electrodes to detect clenching and uses an audio biofeedback signal delivered through bone conduction. Their introduction page provides a good overview of the device's functionality: [Sleepguard Overview](http://mysleepguard.com/sg-international/how-it-works/overview/).

I purchased the device around March 2011 when it became clear to me that my symptoms may be due to night-time bruxism (but I wasn't 100% sure of that at the time). From my experimentation, I could tell the device was properly detecting clenching and was emitting the biodfeedback tone. However, there were a number of problems:

* Some nights, when waking up, I would discover the device was off, without a way of understanding what happened.
* The device only gives a final *clench count*, similarly to the Grindcare device.
* I could not be completely sure that my body was responding to the biofeedback tone.

Shortly after purchasing the Sleepguard, my dentist made a new mouthguard for me which made clenching more difficult by improving contact between front teeth. When wearing this new mouthguard, the device was unable to register any clenching action (the muscle activation must have looked different when wearing this new mouthguard, maybe appearing weaker to the EMG sensors). I ended up returning the device due to this reason.

The Sleepguard device obtains good reviews on Amazon and seems to be generally popular. It may be a good diagnosis device (to understand whether you are a bruxism sufferer or not). However the data it provides is not detailed enough for me, and the biofeedback signal is not strong enough do deter very serious bruxism cases in my opinion.

[Interview with Lee Weinstein, inventor of the Sleepguard](http://www.tmjhope.org/lee-weinstein-interview/)

StatDDS
-------

{% include image.html scaling="c_scale,w_1024" image="bruxism_devices/statdds.jpg" %}

The [StatDDS Sleep and Bruxism](http://statdds.com/collections/frontpage/products/statdds-sleep-and-bruxism-device) device looks like it is marketed exclusively to dentists and is able to detect sleep bruxism and sleep apnea. The price tag makes it out of reach of individuals but there is a rental program available. I don't have experience with this device but it looks very functional. In my opinion though, serious sufferers of bruxism will want to have a lower-cost device which they can comfortable wear everyday in order to effectively monitor their bruxism over time.


Bruxoff
-------

{% include image.html scaling="c_scale,w_1024" image="bruxism_devices/bruxoff.png" %}

The bruxoff device is a diagnosis-only device which includes EMG sensors and heart-rate sensors. At the time of this writing, their website ([Bruxoff.com](http://www.bruxoff.com)) is unavailable. The product is an ipod-sized worn around the chest, with electrodes going up to the masseter muscles on each sides, as well as electrodes on the chest to measure heart rate. The data is then analyzed using a desktop application which seems to provide extremely detailed second-by-second data.

The inclusion of a heartrate sensor is an interesting addition which may be motivated by [recent research](http://www.ncbi.nlm.nih.gov/pubmed/23626977) indicating there is a clear correlation between heart rate and bruxism events.

* A [Youtube Video](https://www.youtube.com/watch?v=2EFzQXUXDNM) of the Bruxoff device.
* A [Medical paper](http://www.ncbi.nlm.nih.gov/pubmed/24374575) using the device to explore the relationship between heart rate and bruxism.

Bitestrip
---------

{% include image.html scaling="c_scale,w_1024" image="bruxism_devices/bitestrip.jpg" %}

The [Bitestrip](http://www.bitestrip.com/) is a low-cost diagnosis-only device stuck to the masseter muscle at night. At the end of the night, it shows a *bruxism severity level* indicating how severe the patient's bruxism was that night (no bruxism, mild, moderate, severe). The company calls it a *disposable bruxism test* and can apparently only be used for a single night. After that, it must be returned to the dentist. 

Bruxlab
-------

{% include image.html scaling="c_scale,w_1024" image="bruxism_devices/bruxlab.jpg" %}

[Bruxlab](http://www.bruxlab.com/) is mobile application which detects bruxism by identifying sounds. It uses special filtering to identify grinding sounds. I haven't tried this app but it sounds like an easy solution to try out given everyone has a smartphone. The FAQ does indicate loud fans or air conditioners may interfere with the results.

<p style="margin-bottom: 50px;"></p>

Devices in development / research devices
=========================================

Bruxane
-------

{% include image.html scaling="c_scale,w_1024" image="bruxism_devices/bruxane.jpg" %}

The [Bruxane](http://www.bruxane.com/) (website in German only) device is a custom-built mouthguard with a built-in vibration motor. This approach is interesting and all integrated in the mouth with no wires around the face. 

The website is not very explicit about the ordering process, but I can imagine it won't be cheap, given the need to custom-mold the mouthguard and fit the electronics inside. It's possible that it'll only be available through a dentist.

[English-language overview](http://www.precisionmicrodrives.com/tech-blog/2014/11/06/vibration-motors-used-to-combat-bruxism)

Smart Mouthguard
----------------

{% include image.html scaling="c_scale,w_1024" image="bruxism_devices/smart-mouth-guard.jpg" %}

This [Smart Mouthguard](http://www.gizmag.com/bruxism-detecting-mouth-guard/38328/) is a university project consisting of a mouthguard fitted with force sensors, relaying information to a smartphone app via bluetooth. [More Information](http://news.ufl.edu/archive/2015/07/smart-mouth-guard-could-detect-teeth-grinding-dehydration-concussions.html#prettyPhoto)

Grindcare V2
------------

{% include image.html scaling="c_scale,w_1024" image="bruxism_devices/grindcare_v2.jpg" %}

The new [Grindcare](http://www.grindcare.com) device looks like a very compact bruxism device which presumably uses the same electric stimulus as the previous iteration of the device. The very small size and the lack of wires could make it a very convenient and comfortable to wear device. Unfortunately the website doesn't include any information about availability.

Grindbit
--------

{% include image.html scaling="c_scale,w_1024" image="bruxism_devices/grindbit.png" %}

The [Grindbit](http://grindbit.launchrock.com/) is a headband device in development by Peter, a member of the [Bruxhackers](https://groups.google.com/forum/#!forum/bruxhackers) mailing list. Some more information available [here](https://community.particle.io/t/im-building-a-night-time-bruxism-sleep-tracking-wearable-device/16840/6?u=dustpuppy). It uses an interesting form factor and some kind of rubberized material.

<p style="margin-bottom: 50px;"></p>

DIY Devices
===========

These devices are made by individuals who have shared the blueprints indicating how to build them.

Grind Control
-------------

{% include image.html scaling="c_scale,w_1024" image="20130601-P6010011.jpg" %}

The [Grind Control Device](https://github.com/lucwastiaux/gc) is my original bruxism device, built in 2013, using an Arduino Micro, the Muscle Sensor v3 and a microSD card reader. [Development of Grind Control Device](/2013/04/01/development-of-grind-control-device/). Instructions on how to build are available on [Github](https://github.com/lucwastiaux/gc). A few people on the Bruxhackers mailing list have built the device for themselves.

Chompr
------

{% include image.html scaling="c_scale,w_1024" image="bruxism_devices/chompr.jpg" %}

The [Chompr](http://chompr.blogspot.hk/search/label/compr-movr) device is built by Anthony, also a member of the Bruxhackers mailing list. It tracks sleep position in an attempt to find a correlation between bruxism and head position. Arduino code is available on [Github](https://github.com/anthonyadams/).

<p style="margin-bottom: 50px;"></p>

Plans for Sleeptrack device
===========================

With so many devices on the market or in development, why build another one ? Here are some of the ways in which Sleeptrack is different:

* **Open source, extensible platform**: both the hardware and software (device firmware, web app and analytics) will be open source. Also, the InfluxDB data repository which stores the time-series data generated by the device will be available for anyone to run queries against.
* **Wireless and cloud-enabled**: while as some of the devices listed above are offline-only, only providing some data through their LCD screens, some allow PC connectivity, but the data remains inside their proprietary app. The sleeptrack device will upload data to the cloud throughout the night and allow consulting the results through the webapp. This convenience allows regular monitoring and doesn't require the user to periodically download the data.
* **Extremely detailed data**: while a nighly summary of bruxism activity is provided, the user can know exactly when bruxism activity happened, when head movement happened. The sleeptrack device measures all activity 10 times/second and publishes all the data. Anyone who has an analytical mind should enjoy using this device. This is in contrast to the Grindcare and Sleepguard devices which only provide number of grind events.
* **IMU on board**: the device has an IMU (Inertial Measurment Unit) which tracks the position of the head throughout the night. In addition to providing sleeping position data, it will help determine whether the user is asleep or woken up.
* **Audio and Vibration alerting**: the device will have a configurable alarm threshold and provide a choice of audio alerting through headphones, with configurable volume (quiet to very loud) which will make it impossible for the user to ignore the alert. A vibration feature is currently in development. Whether bruxism responds well to biofeedback conditionning or not is difficult to prove. However it's abundantly clear that for some patients, the bruxism activity must be stopped at all costs (even at the cost of sleep quality), and the alerting feature on the Sleeptrack is built for that purpose.
* **Worn on headband**: the device is worn on a headband with the EMG sensor connected by a wire. All of the hardware is worn on the head. This allows for free neck movement, as there are no wires going to other parts of the body. Other devices worn around the neck tend to constrain head movement for fear of pulling on the wires.