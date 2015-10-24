/******************************************************************************
SparkFunMAX17043.h
SparkFun MAX17043 Library Header File
Jim Lindblom @ SparkFun Electronics
Original Creation Date: June 22, 2015
https://github.com/sparkfun/SparkFun_MAX17043_Particle_Library

This file implements all functions of the MAX17043 class. Functions here range
from higher level stuff, like reading/writing MAX17043 registers to low-level,
hardware reads and writes.

Development environment specifics:
	IDE: Particle Build
	Hardware Platform: Particle Photon
	                   SparkFun Photon Battery Shield

This code is released under the MIT license.

Distributed as-is; no warranty is given.
******************************************************************************/
#ifndef __SparkFunMAX17043_H__
#define __SparkFunMAX17043_H__

#include "application.h"

class MAX17043
{
public:
  MAX17043();

  // begin() - Initializes I2C and the MAX17043.
  uint8_t begin();

  // quickStart() - Restarts the MAX17043 to allow it to re-"guess" the
  // parameters that go into its SoC algorithms. Calling this in your setup()
  // usually results in more accurate SoC readings.
  // Output: 0 on success, positive integer on fail.
  uint8_t quickStart();

  // getVoltage() - Get the MAX17043's voltage reading.
  // Output: floating point value between 0-5V in 1.25mV increments.
  float getVoltage();

  // getSOC() - Get the MAX17043's state-of-charge (SOC) reading, as calculated
  // by the IC's "ModelGauge" algorithm.
  // Output: floating point value between 0-100, representing a percentage of
  // full charge.
  float getSOC();

  // getVersion() - Get the MAX17043's production version number.
  // Output: 3 on success
  uint16_t getVersion();

  // getThreshold() - Get the MAX17043's current percentage threshold that will
  // trigger an alert.
  // Output: An integer value between 1 and 32, representing a % that will
  // trigger an alert interrupt.
  uint8_t getThreshold();

  // setThreshold([percent]) - Set the MAX17043's percentage threshold that will
  // trigger an alert.
  // Input: [percent] - Percentage value that will trigger an alert interrupt.
  // Any value between 1 and 32 is valid.
  // Output: 0 on success, positive integer on fail.
  uint8_t setThreshold(uint8_t percent);

  // getAlert([clear]) - Check if the MAX17043's alert interrupt has been
  // triggered.
  // INPUT: [clear] - If [clear] is true, the alert flag will be cleared if it
  // was set.
  // OUTPUT: Returns 1 if interrupt is/was triggered, 0 if not.
  uint8_t getAlert(bool clear = false);

  // clearAlert() - Clear the MAX17043's alert flag.
  // Output: 0 on success, positive integer on fail.
  uint8_t clearAlert();
  
  // sleep() - Set the MAX17043 into sleep mode.
  // Output: 0 on success, positive integer on fail.
  uint8_t sleep();

  // wake() - Wake the MAX17043 up from sleep.
  // Output: 0 on success, positive integer on fail.
  uint8_t wake();

  // reset() - Issue a Power-on-reset command to the MAX17043. This function
  // will reset every register in the MAX17043 to its default value.
  // Output: 0 on success, positive integer on fail.
  uint8_t reset();

  // getConfigRegister() - Read the 16-bit value of the CONFIG Register.
  // Output: 16-bit integer value representing the msb and lsb bytes of the
  // CONFIG register.
  uint16_t getConfigRegister();

  // getCompensation() - Get the ModelGauge compensation value - an obscure
  // 8-bit value set to 0x97 by default.
  // Output: 8-bit value read from the CONFIG register's MSB.
  uint8_t getCompensation();

  // setCompensation([newCompensation]) - Set the 8-bit compensation value. This
  // is an obscure 8-bit value that has some effect on Maxim's ModelGauge
  // algorithm.
  // From the datasheet: "Contact Maxim for instructions for optimization."
  // Input: [newCompensation] - Should be a value between 0-255.
  // Output: 0 on success, positive integer on fail.
  uint8_t setCompensation(uint8_t newCompensation);

private:
  // write16([data], [address]) - Write 16 bits to the requested address. After
  // writing the address to be written, two sequential 8-bit writes will occur.
  // the msb is written first, then lsb.
  // Input: [data] - A 16-bit integer to be written to the device.
  //        [address] - An 8-bit address to be written to.
  // Output: 0 on success, positive integer on fail.
  uint8_t write16(uint16_t data, uint8_t address);

  // read16([address]) - Read 16-bits from the requested address of a device.
  // Input: [address] - An 8-bit address to be read from.
  // Output: A 16-bit value read from the device's address will be returned.
  uint16_t read16(uint8_t address);
};

// Define a static MAX17043 object called lipo, which we'll use in the sketches.
extern MAX17043 lipo;

#endif
