---
layout: post
title: "A first look at the SleepTrack Web Interface"
section: blog
thumbnail: https://res.cloudinary.com/photozzap/image/upload/c_fill,g_north_westt,h_400,w_400/v1460198263/gc_website_blog/first_look_sleeptrack_interface/latest_data.jpg
---

A few weeks ago, I posted an <a href="/2016/03/11/progress-update-pcb-prototype/">update on the status of the SleepTrack device</a>. I am currently waiting for revision 3 of the printed circuit board for testing. While I am hoping this revision will be functional and will allow me to start capturing data at night, the difficulties encountered in the hardware portion of this project have surprised me, but are not unusual in a hardware project. As the saying goes, hardware is hard. With that said, let's take a look at the SleepTrack web interface.

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

The **Control** tab provides two options to take control of your device:

* **Realtime mode** will stream data directly to the web application, in realtime. The main purpose of this mode is to verify the muscle sensor electrode placement. 
* **Night mode** allows you to start recording data for the night, which will be uploaded in batches.

Also notice the **device status** on the upper right-hand corner. You can see that your device is online, and its battery is 100% charged.

{% include image.html scaling="" image="first_look_sleeptrack_interface/realtime_mode_checks.jpg" %}

Before entering realtime mode, a number checks will be performed, such as verifying that the device is reachable, and that the initial setup has been done.

{% include image.html scaling="" image="first_look_sleeptrack_interface/realtime_mode_data.jpg" %}

**Realtime mode**, showing the current EMG sensor value in the gauge on the left, as well as a short history on the chart on the right. Contracting the temporal muscle should show a spike on this chart. At rest, the chart should show a low value. Currently this only shows EMG/Muscle sensor data, but in the future it will show accelerometer data as well.

{% include image.html scaling="" image="first_look_sleeptrack_interface/latest_data.jpg" %}

This page shows **latest data** captured during night mode. This page will auto-refresh everytime the SleepTrack device uploads data, which should be around every 15mn. This currently only shows EMG data. A simplistic *bruxism score* is calculated. This is based on the time-weighted duration of EMG spikes. The formula for calculating this is very primitive currently. This whole page should see a great deal of enhancements in the future, but this will only be done after I have a working prototype that lets me accumulate data while sleeping.

On the upper right hand corner, you can see the device is in **Sleep mode**, and has uploaded 5mn of data so far.

{% include image.html scaling="" image="first_look_sleeptrack_interface/influxdb.jpg" %}

The interface above shows every single datapoint captured by the device. The technological stack making this possible uses the *InfluxDB* database, a datastore specialized for storing *IoT* (internet of things) device data. It also uses the *Grafana* **dashboard**, which is pictured in the screenshot above. SleepTrack users will have access to this dashboard and will be able to analyze their data if a very detailed analysis is required. The dashboard is very customizable, and for example can be used to highlight times at which the biofeedback alarm kicked in. And hopefully it will indicated that bruxism / teeth grinding / clenching stops after triggering the biofedback signal !

While this interface will be extremely powerful, I don't expect every single SleepTrack user will make use of it, unless they have a keen interest in data analysis. In most cases, the data could be summarized in more readable way, with per-night results and a summary over the last weeks or months.

{% include image.html scaling="" image="first_look_sleeptrack_interface/settings.jpg" %}

Finally, this **settings** page allows you to configure your SleepTrack device. Most of the settings only need to be configured once, however more things may be added.

All of the above is work in progress, but the current functionality should allow collecting data at night, and analyzing it the next day. Among the features that are planned, but not yet implemented:

* **Biofeedback settings**: controlling the audio alarm (and vibration alarm, once the hardware is developed), how quickly it comes on, how often, will all be configured from this app.
* **Detailed night data**: number of bruxism events, amount of movement during the night should all be shown in relevant way. There are a lot of ways to show this in a smart way.
* **Historical stats**: these should show how your bruxism, teeth grinding or clenching behavior evolves over time. Effectiveness of the biofeedback should also be reflected.

All of these enhancements will come later. For now, I'm focusing on a small set of functionality, just enough to capture nightly data. In software development, it's generally considered a good thing to deliver a small, but functional project, and progressively enhance it. There is a lot of work left to do, and the progress hinges on having a reliable hardware prototype to start capturing data.