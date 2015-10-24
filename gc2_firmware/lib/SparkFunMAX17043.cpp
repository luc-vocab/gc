/******************************************************************************
SparkFunMAX17043.cpp
SparkFun MAX17043 Library Source File
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
#include "SparkFunMAX17043.h"

///////////////////////////////////
// MAX17043 Register Definitions //
///////////////////////////////////
// All registers contain two bytes of data and span two addresses.
#define MAX17043_VCELL    0x02 // R - 12-bit A/D measurement of battery voltage
#define MAX17043_SOC      0x04 // R - 16-bit state of charge (SOC)
#define MAX17043_MODE     0x06 // W - Sends special commands to IC
#define MAX17043_VERSION  0x08 // R - Returns IC version
#define MAX17043_CONFIG   0x0C // R/W - Battery compensation (default 0x971C)
#define MAX17043_COMMAND  0xFE // W - Sends special comands to IC

///////////////////////////////////
// MAX17043 Config Register Bits //
///////////////////////////////////
#define MAX17043_CONFIG_SLEEP     7
#define MAX17043_CONFIG_ALERT     5
#define MAX17043_CONFIG_THRESHOLD 0

/////////////////////////////////////
// MAX17043 Mode Register Commands //
/////////////////////////////////////
#define MAX17043_MODE_QUICKSTART 0x4000

////////////////////////////////////////
// MAX17043 Command Register Commands //
////////////////////////////////////////
#define MAX17043_COMMAND_POR 0x5400

////////////////////////////////
// MAX17043 7-Bit I2C Address //
////////////////////////////////
#define MAX17043_ADDRESS  0x36

MAX17043::MAX17043()
{

}

uint8_t MAX17043::begin()
{
  Wire.begin();
  return 1;
}

uint8_t MAX17043::quickStart()
{
  // A quick-start allows the MAX17043 to restart fuel-gauge calculations in the
  // same manner as initial power-up of the IC. If an application’s power-up
  // sequence is exceedingly noisy such that excess error is introduced into the
  // IC’s “first guess” of SOC, the host can issue a quick-start to reduce the
  // error. A quick-start is initiated by a rising edge on the QSTRT pin, or
  // through software by writing 4000h to MODE register.
  return write16(MAX17043_MODE_QUICKSTART, MAX17043_MODE);
}

float MAX17043::getVoltage()
{
  uint16_t vCell;
  vCell = read16(MAX17043_VCELL);
  // vCell is a 12-bit register where each bit represents 1.25mV
  vCell = (vCell) >> 4;

  return ((float) vCell / 800.0);
}

float MAX17043::getSOC()
{
  uint16_t soc;
  float percent;
  soc = read16(MAX17043_SOC);
  percent = (soc & 0xFF00) >> 8;
  percent += (float) (((uint8_t) soc) / 256.0);

  return percent;
}

uint16_t MAX17043::getVersion()
{
  return read16(MAX17043_VERSION);
}

uint8_t MAX17043::getThreshold()
{
  uint16_t configReg = read16(MAX17043_CONFIG);
  uint8_t threshold = (configReg & 0x001F);

  // It has an LSb weight of 1%, and can be programmed from 1% to 32%.
  // Values are in 2's complement, e.g.: 00000=32%, 00001=31%, 11111=1%.
  // Let's convert our percent to that first:
  threshold = 32 - threshold;
  return threshold;
}

uint8_t MAX17043::setThreshold(uint8_t percent)
{
  // The alert threshold is a 5-bit value that sets the state of charge level
  // where an interrupt is generated on the ALRT pin.

  // It has an LSb weight of 1%, and can be programmed from 1% to 32%.
  // Values are in 2's complement, e.g.: 00000=32%, 00001=31%, 11111=1%.
  // Let's convert our percent to that first:
  percent = constrain(percent, 0, 32);
  percent = 32 - percent;

  // Read config reg, so we don't modify any other values:
  uint16_t configReg = read16(MAX17043_CONFIG);
  configReg &= 0xFFE0; // Mask out threshold bits
  configReg |= percent; // Add new threshold

  return write16(configReg, MAX17043_CONFIG);
}

uint8_t MAX17043::clearAlert()
{
  // Read config reg, so we don't modify any other values:
  uint16_t configReg = read16(MAX17043_CONFIG);
  configReg &= ~(1<<6); // Clear ALRT bit manually.

  return write16(configReg, MAX17043_CONFIG);
}

uint8_t MAX17043::getAlert(bool clear)
{
  // Read config reg, so we don't modify any other values:
  uint16_t configReg = read16(MAX17043_CONFIG);
  if (configReg & (1<<6))
  {
    if (clear) // If the clear flag is set
    {
      configReg &= ~(1<<6); // Clear ALRT bit manually.
      write16(configReg, MAX17043_CONFIG);
    }
    return 1;
  }

  return 0;
}

uint8_t MAX17043::sleep()
{
  // Read config reg, so we don't modify any other values:
  uint16_t configReg = read16(MAX17043_CONFIG);
  if (configReg & (1<<7))
    return 5; // Already sleeping, do nothing but return an error
  configReg |= (1<<7); // Set sleep bit

  return write16(configReg, MAX17043_CONFIG);
}

uint8_t MAX17043::wake()
{
  // Read config reg, so we don't modify any other values:
  uint16_t configReg = read16(MAX17043_CONFIG);
  if (!(configReg & (1<<7)))
    return 5; // Already awake, do nothing but return an error
  configReg &= ~(1<<7); // Clear sleep bit

  return write16(configReg, MAX17043_CONFIG);
}

uint8_t MAX17043::reset()
{
  return write16(MAX17043_COMMAND_POR, MAX17043_COMMAND);
}

uint8_t MAX17043::getCompensation()
{
  uint16_t configReg = read16(MAX17043_CONFIG);
  uint8_t compensation = (configReg & 0xFF00) >> 8;
  return compensation;
}

uint16_t MAX17043::getConfigRegister()
{
  return read16(MAX17043_CONFIG);
}

uint8_t MAX17043::setCompensation(uint8_t newCompensation)
{
  // The CONFIG register compensates the ModelGauge algorith. The upper 8 bits
  // of the 16-bit register control the compensation.
  // Read the original configReg, so we can leave the lower 8 bits alone:
  uint16_t configReg = read16(MAX17043_CONFIG);
  configReg &= 0x00FF; // Mask out compensation bits
  configReg |= ((uint16_t)newCompensation << 8) | configReg;
  return write16(configReg, MAX17043_CONFIG);
}

uint8_t MAX17043::write16(uint16_t data, uint8_t address)
{
  uint8_t msb, lsb;
  msb = (data & 0xFF00) >> 8;
  lsb = (data & 0x00FF);
  Wire.beginTransmission(MAX17043_ADDRESS);
  Wire.write(address);
  Wire.write(msb);
  Wire.write(lsb);
  return (Wire.endTransmission());
}

uint16_t MAX17043::read16(uint8_t address)
{
  uint8_t msb, lsb;
  int16_t timeout = 1000;

  Wire.beginTransmission(MAX17043_ADDRESS);
  Wire.write(address);
  Wire.endTransmission(false);

  Wire.requestFrom(MAX17043_ADDRESS, 2);
  while ((Wire.available() < 2) && (timeout-- > 0))
    delay(1);
  msb = Wire.read();
  lsb = Wire.read();

  return ((uint16_t) msb << 8) | lsb;
}

// Define a static MAX17043 object called lipo, which we'll use in the sketches.
MAX17043 lipo;
