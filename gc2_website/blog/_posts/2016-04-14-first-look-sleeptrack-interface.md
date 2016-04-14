---
layout: post
title: "A first look at the SleepTrack Web Interface"
section: blog
thumbnail: https://res.cloudinary.com/photozzap/image/upload/c_fill,g_north_westt,h_400,w_400/v1460198263/gc_website_blog/first_look_sleeptrack_interface/latest_data.jpg
---

A few weeks ago, I posted an <a href="/2016/03/11/progress-update-pcb-prototype/">update on the status of the SleepTrack device</a>. I am currently waiting for revision 3 of the printed circuit board for testing. While I am hoping this revision will be functional and will allow me to start capturing data at night, the difficulties encountered in the hardware portion of this project have surprised me to say the least. As the saying goes, hardware is hard. With that said, let's take a look at the SleepTrack web interface.

{% include image.html scaling="" image="first_look_sleeptrack_interface/interface_tabs.jpg" %}
*The various tabs on this website provide a preview of the different functionality available on the SleepTrack web app.*

As with everything, the hardware and software go hand in hand. One of the <a href="/2015/10/01/designing-new-device/">stated goals of SleepTrack</a> is to have a completely wireless device. The Grind Control prototype I built in 2013 wrote all of its data to a MicroSD card. This design worked well but had several limitations:

* **EMG Electrode placement:** because the device had no display, it wasn't possible to easily verify that the EMG electrodes were placed properly and that the muscle sensor readings were correct when setting up the device at the beginning of the night. This was mitigated by a primitive selectable *beep mode* which beeped through the earphones when EMG levels crossed a certain threshold.
* **Data analysis** at the end of a night was only feasible by opening up the device, extracting the printed circuit board, removing the MicroSD card, copying its content to my computer, then analyzing the data using a python script. This easily took 20mn. So not something I could do every day as part of my morning routine.
* **Configuration:** most importantly, because of the lack of a proper interface to the device, things like configuration, sensitivity of the alarm were all hard-coded in the Arduino code. While the preset values worked well for me, a device used by different people will need to be customized to the user, particularly around things like muscle sensor sensitivity, alarm levels (do you want the biofeedback to kick-in immediately, or only after a certain duration of grinding/clenching). This requires a proper user interface.

The approach I decided on to solve the above issues is to go with an internet-enabled micro-controller. The Particle Photon used inside the SleepTrack device is a wifi chip. All of its functionality can be controlled over wifi, specifically over a web application. And to me this makes a lot of sense for the following reasons:

* **No LCD monitor:** because the device is worn on the side of the head, even if it had an LCD screen, you wouldn't be able to see it. Because the device will not be visible to you most of the time, being able to control it from your smartphone, tablet or PC makes a lot of sense.
* **Amount of data:** while some of the other bruxism devices such as the Grindcare only expose limited data to the user such as *number of grinds per night*, the SleepTrack captures **all** of the data, namely EMG sensor data and accelerometer motion, several times a second. The data is uploaded periodically over wifi. Upon waking up, there is no need to "upload" any data, as that will have happened throughout the night. All of this requires a sophisticated interface to display the data. 
* **Configuration:** configuring the device, such as changing the biofeedback settings, can be done in a user friendly way.
* **Firmware upgrades:** the firmware can be upgraded over the air. This is an important feature as it allows me to fix bugs, or add functionality even after shipping the device.

In practice, you will use the web interface when setting up the device before sleep, which can be done from a **Smartphone, Tablet or PC**. After your device is setup in *night mode*, you may turn off your smartphone/tablet, and the SleepTrack device will periodically upload data directly over Wifi. So as long as your Wifi router is working, your data will be uploaded to the cloud and visible any time, including right when you wake up.

On the flipside, the device will require a reliable internet connection. This could be an issue for example when going on vacation. This problem can be mitigated by introducing an offline mode, though that will come later, as I believe it's a fair expectation that most households are equipped with Wifi internet these days.

{% include image.html scaling="" image="first_look_sleeptrack_interface/control_tab.jpg" %}

{% include image.html scaling="" image="first_look_sleeptrack_interface/realtime_mode_checks.jpg" %}

{% include image.html scaling="" image="first_look_sleeptrack_interface/realtime_mode_data.jpg" %}

{% include image.html scaling="" image="first_look_sleeptrack_interface/latest_data.jpg" %}

{% include image.html scaling="" image="first_look_sleeptrack_interface/influxdb.jpg" %}

{% include image.html scaling="" image="first_look_sleeptrack_interface/settings.jpg" %}