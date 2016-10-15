---
layout: post
title: "Observations from wearing the Sleeptrack prototype"
section: blog
thumbnail: https://res.cloudinary.com/photozzap/image/upload/c_fill,g_north,h_400,w_400/v1468048450/gc_website_blog/observations_1/article_header.png
---

In my <a href="/2016/07/09/phase1-hardware/">previous post</a>, I showed the sleeptrack phase1 prototype, which I've been wearing on and off at night. Here are my observations about the device, and about my own night-time teeth grinding behavior. I have to say I was shocked at how much muscle activity is taking place in a single night.

In its current state, the prototype is a passive device which only collects data about muscle activity. It samples the EMG sensor 10 times per second. I've worn the EMG (muscle sensor) on the temporal and masseter muscles. Both configurations work, however there is discussion on the bruxhackers mailing list suggesting the masseter muscle should give a cleaner output with less interference from other facial muscles.

Comfort
-------

One of my biggest complains with the <a href="/2016/02/23/review-of-bruxism-devices/">Grindcare</a> or my 2013 <a href="/2013/06/01/experiences-with-grind-control-device/">Grind Control device</a> was the wire going from the device worn on the front of the body to the electrodes on the temporal muscle. The head and body move independently which causes a strain on that wire and restricts freedom of movement. Sometimes at night, the main device would shift around, pulling on the electrode cable which made me worry about damaging the cable.

Sleeptrack on the other hand, consists of a main device worn on a headband on the side of the head, and of the muscle sensor, worn also on the head, on the temporal or masseter muscles. Because these two parts of the body don't move independently, there is no strain on the cable, and the head is free to move without interference from the device. On this aspect, the design of sleeptrack fulfills its goal.

The phase1 prototype is a bit larger than what I expect the final version to be. When wearing the device on my right side, when leaning my head to the right slightly, I can feel it pressing against my pillow. Also, the flat shape of the device feels a bit unnatural against the curved head. These aspects should improve as the shape of the device changes in future revisions. Though nothing will ever be as comfortable as sleeping without electronics attached to the head unfortunately.


Setup process before sleeping
-----------------------------

By far the biggest problem that I run into is achieving proper placement of the electrodes on the muscles. Incorrect placement or insufficient adherence causes the EMG readings to be completely off. I've found that surgical tape is necessary to keep the electrodes in place. Occasionally, I need several tries to find the right placement which gives me correct EMG readings.

Once the device is installed, the <a href="/2016/04/14/first-look-sleeptrack-interface/">realtime mode</a> on the sleeptrack web interface allows checking of EMG readings in real time during rest as well as during muscle clenching. I'm planning on enhancing this some more, maybe with a semi-automatic EMG reading test to help the user ensure electrode placement is correct every time.

Overall setting up the muscle sensor in the right place is the biggest issue with the device right now. I'm brainstorming to find an adequate solution to this problem.

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
 
 
 


