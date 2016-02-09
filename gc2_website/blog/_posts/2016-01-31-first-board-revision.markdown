---
layout: post
title: "First batch of boards printed, issues with layout"
date: 2016-01-31 00:00:00 +0000
section: blog
thumbnail: http://res.cloudinary.com/photozzap/image/upload/c_fill,h_400,w_400/v1454817859/gc_website_blog/Photo_Jan_30_10_56_58_PM.jpg
---
I've been working with a PCB (Printed Circuit Board) designer for the last two months to put together the board for the Sleeptrack device. The [Grind Control device](2013/04/01/development-of-grind-control-device/) was built on top of breadboard-ready through-hole components which could all be soldered easily. In this new iteration, one of the objectives is to keep the size small. This means using [SMD components](https://en.wikipedia.org/wiki/Surface-mount_technology), and I don't have the skills or equipment to solder those. So PCB design and assembly is contracted out and a few days ago, we received the first revision of the boards. 

<img src="http://res.cloudinary.com/photozzap/image/upload/c_fill,h_1024,w_1024/v1454922913/gc_website_blog/pcb_rounded_corners.jpg" class="img-responsive">

The board is a nice looking 2"x2" (5 centimers square) board. Unfortunately, the cutting wasn't done properly by the manufacturer (our fault, we forgot to include a design file). The above photo is taken after we sanded the corners to round them.

<img src="http://res.cloudinary.com/photozzap/image/upload/c_fill,g_north_west,h_1024,w_1024/v1454817789/gc_website_blog/image1.jpg" class="img-responsive">

The same board after soldering the [Photon P1](https://docs.particle.io/datasheets/p1-datasheet/). The P1 is an SMD version of the Particle Photon, which includes a built-in wifi antenna. This was soldered using a reflow oven. While this step went well, it turns out there were a number of problems with this board.

<img src="http://res.cloudinary.com/photozzap/image/upload/c_scale,w_1024/v1454817867/gc_website_blog/Photo_Jan_30_10_32_37_PM.jpg" class="img-responsive">

* **Silkscreen missing**: most of the markings on the board are missing, making it difficult to assemble components.
* **Some footprints are incorrect**: the speaker footprint is too large. The push button footprints are in an incorrect orientation.
* **RGB LED too close to P1**

Despite these issues, we went ahead and soldered on more components in an attempt to identify more issues:

<img src="http://res.cloudinary.com/photozzap/image/upload/c_scale,w_1024/v1454817859/gc_website_blog/Photo_Jan_30_10_56_58_PM.jpg" class="img-responsive">

On the picture above, you can see:

* **Photon P1** _left side_. This is the heart of the device, with microcontroller and Wifi chip.
* **Speaker**, _lower left_: this small buzzer is used for feedback to the user when turning on or activating certain functions. Feedback is important as the user cannot see the device while wearing it (it will be on the side of their head).
* **3.5mm audio jack**, _bottom_: one of the core functionalities is to have a _bruxism alarm_ which wakes up the user after a predefined threshold of muscle activity has been breached. There is a volume-control circuit to start the alarm at a low volume and gradually increase if the bruxism activity doesn't stop.
* **On/Off switch**, _right side_, red/black: opens/close the battery circuit.
* **EMG connector**, _lower right_: this will be used to connect to the Myoware EMG sensor. The connector is wrong, it should be a right-angled connector.
* **JST Battery connnector**, _upper right_: connects to a LIPO battery. Connector is also wrong.
* **Micro-USB connector**, _top right_: used to charge the device (charging only, as firmware flashing is over the air)
* **Two push-buttons**, _top left_: will be used to control the device, for example, starting data capture after the device has been configured.

A few more pictures with the same components:

<img src="http://res.cloudinary.com/photozzap/image/upload/c_fill,g_center,h_600,w_1024/v1454817858/gc_website_blog/Photo_Jan_30_10_56_21_PM.jpg" class="img-responsive">
<img src="http://res.cloudinary.com/photozzap/image/upload/c_fill,h_500,w_1024/v1454817860/gc_website_blog/Photo_Jan_30_10_56_30_PM.jpg" class="img-responsive">
<img src="http://res.cloudinary.com/photozzap/image/upload/c_fill,w_1024/v1454817859/gc_website_blog/Photo_Jan_30_10_56_36_PM.jpg" class="img-responsive">
<img src="http://res.cloudinary.com/photozzap/image/upload/c_fill,g_south,h_400,w_1024/v1454817859/gc_website_blog/Photo_Jan_30_10_56_44_PM.jpg" class="img-responsive">

While this board has issues, overall it feels like progress is being made, and we're getting ready to submit revision 2. We will also try to have the assembly done by an assembly house as we feel they'll be able to assemble boards faster, even at this stage of the project.