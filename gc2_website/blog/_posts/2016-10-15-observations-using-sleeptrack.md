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

This is the really interesting part. Ever since it became clear to me that my <a href="/2012/10/01/symptoms-and-diagnosis/">symptoms</a> were the result of unconscious night-time muscle activity, I've been looking for ways to analyze this mysterious behavior. It's not easy at all to observe your own sleeping behvior. Some people go as far as video-recording themselves with an infrared camera. Something i've considered doing, and that other people on the bruxhackers mailing list have done.

{% include image.html scaling="c_fill,w_1024" image="observations_1/repeated_grinds.png" %}

The above chart shows muscle activity over a 4.5mn period and is representative of the whole night's activity overall. Values around 140-150 show that muscles are at rest. Values above that, going from 180 to up to 600 indicate the muscles are contracted. Most of the activity happens in periods of 30 seconds to 60 seconds. There are dozens of these clusters of activity happening throughout the night.

{% include image.html scaling="c_fill,w_1024" image="observations_1/30s_grind.png" %}

Above chart shows a 30 second period of activity. The seesaw pattern indicates side-to-side grinding as facial muscles on either side of the face take turn in contracting and releasing. If we had a muscle sensor on the other side of the head, it would show opposite activity (contract while the other side is at rest).

{% include image.html scaling="c_fill,w_1024" image="observations_1/1mn_grind.png" %}

Above we have a 1mn grinding activity, with the same seesaw pattern. There is a short pause between the different periods of activity. All of this is happening unconscously during my sleep, so it's hard to say what I had in mind and why the muscle activity starts and stops this way.

Necessary improvements
----------------------

Having accumulated some experience with wearing the device, a couple of things are becoming clear to me.

* The device needs **additional sensors** to gather more information about facial movements. This will come the form of the dual-IMU in the subsequent revisions of the device. One of the IMUs will be **attached to the chin** and provide very detailed information about the movement of the jaw, and allow distinguishing of grinding vs clenching patterns.
* It generates a **lot of data**. Just the EMG values, which is the only data being captured right now, are sampled 10 times / second. This means 252,000 datapoints for a 7-hour night. Sorting through this data will required advanced techniques. I'm currently studying what tools in statistics, signal processing and machine learning can be used for that purpose.
* Designing an effective **biofeedback** strategy will require careful thought. The number of bruxism events taking place every night is overwhelming. A naive biofeedback strategy which wakes up / signals the user every single time would be extremely tough to endure and damage sleep quality, though someone wanting to stop bruxism at any cost might still want exactly this. In my case, the main thing I care about is managing day-time symptoms. Hence I'll look to control the total accumulated bruxism time to under an acceptable level which doesn't cause day-time headaches or dizziness or muscle pain. Different people will want to configure their biofeedback differently, so that part will need to be flexible. This is something that Grindcare doesn't allow.
 


