---
layout: post
title: "Observations from wearing the Sleeptrack prototype"
section: blog
thumbnail: https://res.cloudinary.com/photozzap/image/upload/c_fill,g_north,h_400,w_400/v1468048450/gc_website_blog/observations_1/article_header.png
---

In my <a href="/2016/07/09/phase1-hardware/">previous post</a>, I showed the sleeptrack phase1 prototype, which I've been wearing on and off at night. Here are my observations about the device, and about my own night-time teeth grinding behavior. I have to say I was shocked at how much muscle activity is taking place in a single night.

Comfort
-------

Setup process before sleeping
-----------------------------

Grinding patterns
-----------------

{% include image.html scaling="c_fill,w_1024" image="observations_1/repeated_grinds.png" %}
{% include image.html scaling="c_fill,w_1024" image="observations_1/30s_grind.png" %}
{% include image.html scaling="c_fill,w_1024" image="observations_1/1mn_grind.png" %}


Necessary improvements
----------------------

After facing difficulties in the previous months trying to design and manufacture the final version of the Sleeptrack Printed Circuit Board, we decided to take a step by step approach and build incremental versions to make more steady progress. The first version, **phase 1** is comprised of the bare minimum components required to measure night-time muscle activity.

{% include image.html scaling="c_fill,w_1024" image="phase1_hardware/sleeptrack_phase1_components.jpg" %}

On the picture above, you can see:

* **Sleeptrack Phase 1 PCB** (Printed Circuit Board) with a Photon Headerless microcontroller, battery charging circuit and EMG connector. This version is a little larger than the final version.
* **1000mah battery** the capacity is sufficient to last over a night.
* **Myoware EMG sensor** with a 3d printed cover to prevent touching the sensitive EMG circuitry.

It doesn't feature any of the other sensors that I wanted, IMU/Accelerometer and chin sensor. These will come in subsequent revisions.

{% include image.html scaling="c_fill,w_1024" image="phase1_hardware/3d_printed_case_1.jpg" %}

The 3d-printed case housing the PCB and battery. Infortunately the nylon screws I had were too short and couldn't close down the case, so I had to use black electrical tape.

{% include image.html scaling="c_fill,w_1024" image="phase1_hardware/3d_printed_case_2.jpg" %}

The case has a special grove to place the headband which secures the device to the head. The assembled case + PCB + battery unit is not heavy, but it's a little bit big considering it's worn on the side of the head. I'm studying ways to shrink down the whole assembly for future revisions. I may design a split case which houses the PCB and battery separately, which will allow me to design something really small and close-fitting.

<div class="row">
<img src="https://res.cloudinary.com/photozzap/image/upload/c_scale,h_1024/v1454745964/gc_website_blog/phase1_hardware/sleeptrack_phase1_headshot_1.jpg" class="col-lg-6 col-md-6 img-responsive">
<img src="https://res.cloudinary.com/photozzap/image/upload/c_scale,h_1024/v1454745965/gc_website_blog/phase1_hardware/sleeptrack_phase1_headshot_2.jpg" class="col-lg-6 col-md-6 img-responsive">
</div>

And here is the assembled device, worn on the side of the head. First observation, yes it's a little bit bulky and not very aesthetic. This is the first fully working protoype, so there is a lot of room for improvement. Both the size and aesthetics will improve, however my priority for now is ensuring that the device is functional.

I've used this version of the device in realtime mode to ensure EMG readings are correct, but haven't worn it for a full night yet. This testing will take place over the coming weeks, and I will share the results here. 
 
Having this version ready is a bit of a milestone for me as the last six months have had a lot of setbacks with manufacturing of the PCB, and it's nice to have a first version working, however basic.
 
 
 


