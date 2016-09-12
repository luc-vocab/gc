IMU Extension Board
===================

The objective is to create a very small IMU board which will be connected to the main Sleeptrack board via I2C.

* We can use this connector http://www.digikey.com/product-search/en?mpart=61900411021&v=732 
* The 4 pins will have to match pin-for-pin with the I2C extension port on the sleeptrack board
* We can use the same chip as for the main board (LSM9DS1) as there are working libraries and you already have the footprint
* Schematic here https://cdn.sparkfun.com/datasheets/IoT/Photon-IMU-Shield-v10.pdf
* SDO_M and SDO_A/G should be connected to **Low** so that we can use I2C addresses 0x6A / 0x1C (see schematic)
* We only need  GND, 3.3V, SDA, SCL (these will be the 4 pins going to the connector)
* The 4-pin connector is 1cm in width. I'm wondering whether 1cm x 1cm is possible for this board.


Questions
---------
* Any concerns with having wires 20cm long on the I2C bus ? 
* Do we need pull up / pull down resistors on the board itself ?
