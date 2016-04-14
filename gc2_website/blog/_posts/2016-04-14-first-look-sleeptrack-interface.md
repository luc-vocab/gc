---
layout: post
title: "A first look at the SleepTrack Web Interface"
section: blog
thumbnail: https://res.cloudinary.com/photozzap/image/upload/c_fill,g_north,h_400,w_400/v1460198263/gc_website_blog/you_may_suffer_bruxism/headache.jpg
---

A few weeks ago, I posted an <a href="/2016/03/11/progress-update-pcb-prototype/">update on the status of the SleepTrack device</a>. I am currently waiting for revision 3 of the printed circuit board for testing. While I am hoping this revision will be functional and will allow me to start capturing data at night, the difficulties encountered in the hardware portion of this project have surprised me to say the least. As the saying goes, hardware is hard. With that said, let's take a look at the SleepTrack web interface.

As with everything, the hardware and software go hand in hand. One of the <a href="/2015/10/01/designing-new-device/">stated goals of SleepTrack</a> is to have a completely wireless device. The Grind Control prototype I built in 2013 wrote all of its data to a MicroSD card. This design worked well but had several limitations:

* Because the device had no display, it wasn't possible to easily verify that the EMG electrodes were placed properly and that the muscle sensor readings were correct when setting up the device at the beginning of the night. This was mitigated by a primitive selectable *beep mode* which beeped through the earphones when EMG levels crossed a certain threshold.
* Data analysis at the end of a night was only feasible by opening up the device, extracting the printed circuit board, removing the MicroSD card, copying its content to my computer, then analyzing the data using a python script. This easily took 20mn. So not something I could do every day as part of my morning routine.
* Most importantly, because of the lack of a proper interface to the device, things like configuration, sensitivity of the alarm were all hard-coded in the Arduino code. While the preset values worked well for me, a device used by different people will need to be customized to the user, particularly around things like muscle sensor sensitivity, alarm levels (do you want the biofeedback to kick-in immediately, or only after a certain duration of grinding/clenching). This requires a proper user interface.

The approach I decided on to solve the above issues is to go with an internet-enabled micro-controller. The Particle Photon used inside the SleepTrack device is a wifi chip. All of its functionality can be controlled over wifi.
* 

{% include image.html scaling="" image="first_look_sleeptrack_interface/interface_tabs.jpg" %}

{% include image.html scaling="" image="first_look_sleeptrack_interface/control_tab.jpg" %}

{% include image.html scaling="" image="first_look_sleeptrack_interface/realtime_mode_checks.jpg" %}

{% include image.html scaling="" image="first_look_sleeptrack_interface/realtime_mode_data.jpg" %}

{% include image.html scaling="" image="first_look_sleeptrack_interface/latest_data.jpg" %}

{% include image.html scaling="" image="first_look_sleeptrack_interface/influxdb.jpg" %}

{% include image.html scaling="" image="first_look_sleeptrack_interface/settings.jpg" %}