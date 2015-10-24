
/******************************************************************************
MAX17043_Simple_Serial.cpp
SparkFun MAX17043 Example Code
Jim Lindblom @ SparkFun Electronics
Original Creation Date: June 22, 2015
https://github.com/sparkfun/SparkFun_MAX17043_Particle_Library

This file demonstrates the simple API of the SparkFun MAX17043 Particle library.
Pair the Photon up to a SparkFun Photon Battery Shield
(https://www.sparkfun.com/products/13626), and away you go!

This example will print the gauge's voltage and state-of-charge (SOC) readings
to both serial (9600 baud) and out to the Particle cloud. Navigate to
https://api.particle.io/v1/devices/{DEVICE_ID}/voltage?access_token={ACCESS_TOKEN}
https://api.particle.io/v1/devices/{DEVICE_ID}/soc?access_token={ACCESS_TOKEN}
https://api.particle.io/v1/devices/{DEVICE_ID}/alert?access_token={ACCESS_TOKEN}

And read your Photon's battery charge from the Cloud!

Development environment specifics:
	IDE: Particle Build
	Hardware Platform: Particle Photon
	                   SparkFun Photon Battery Shield

This code is released under the MIT license.

Distributed as-is; no warranty is given.
******************************************************************************/

#include "SparkFunMAX17043.h" // Include the SparkFun MAX17043 library

double voltage = 0; // Variable to keep track of LiPo voltage
double soc = 0; // Variable to keep track of LiPo state-of-charge (SOC)
bool alert; // Variable to keep track of whether alert has been triggered

void setup()
{
	Serial.begin(9600); // Start serial, to output debug data

	// Set up Spark variables (voltage, soc, and alert):
	/*
	Particle.variable("voltage", &voltage, DOUBLE);
	Particle.variable("soc", &soc, DOUBLE);
	Particle.variable("alert", &alert, INT);
	*/
	// To read the values from a browser, go to:
	// http://api.particle.io/v1/devices/{DEVICE_ID}/{VARIABLE}?access_token={ACCESS_TOKEN}

	// Set up the MAX17043 LiPo fuel gauge:
	lipo.begin(); // Initialize the MAX17043 LiPo fuel gauge

	// Quick start restarts the MAX17043 in hopes of getting a more accurate
	// guess for the SOC.
	lipo.quickStart();

	// We can set an interrupt to alert when the battery SoC gets too low.
	// We can alert at anywhere between 1% - 32%:
	lipo.setThreshold(20); // Set alert threshold to 20%.
}

void loop()
{
	// lipo.getVoltage() returns a voltage value (e.g. 3.93)
	voltage = lipo.getVoltage();
	// lipo.getSOC() returns the estimated state of charge (e.g. 79%)
	soc = lipo.getSOC();
	// lipo.getAlert() returns a 0 or 1 (0=alert not triggered)
	alert = lipo.getAlert();

	// Those variables will update to the Spark Cloud, but we'll also print them
	// locally over serial for debugging:
	Serial.print("Voltage: ");
	Serial.print(voltage);  // Print the battery voltage
	Serial.println(" V");

	Serial.print("Alert: ");
	Serial.println(alert);

	Serial.print("Percentage: ");
	Serial.print(soc); // Print the battery state of charge
	Serial.println(" %");
	Serial.println();

	// Particle.publish("battery_stats", String(soc) + "_" + String(voltage));

	delay(3000);
}
