
// purpose: collect data and upload to gc2 server at 5mn intervals

#include "SparkFunMAX17043.h" // Include the SparkFun MAX17043 library, battery gauge
#include "SparkFunLSM9DS1.h" // include Sparkfun LSM9DS1 library, IMU
#include "math.h"

double battery_voltage = 0;
double battery_soc = 0;
bool serial_debug = true;

TCPClient gc_client;

char send_buffer[500];

// declare functions
int gc_server_connect(String command);
int gc_server_disconnect(String command);
int gc_server_send_data(String command);
int gc_server_send_data_point(String command);

void setup() {
  Particle.function("gc_conn", gc_server_connect);
  Particle.function("gc_disc", gc_server_disconnect);
  Particle.function("gc_send_1", gc_server_send_data);
  Particle.function("gc_send_2", gc_server_send_data_point);


  if (serial_debug) {
    Serial.begin(9600);
  }

  // setup battery gauge
  lipo.begin();
  lipo.quickStart();

  Particle.publish("startup");

}

int gc_server_connect(String command) {
  if(serial_debug) {
      Serial.println("gc_server_connect");
  }

  gc_client.connect("dev2.photozzap.com", 7001);

  return 0;
}

int gc_server_disconnect(String command) {
  if(serial_debug) {
      Serial.println("gc_server_disconnect");
  }

  gc_client.stop();

  return 0;
}

void write_int_to_buffer(char *buffer, int number, int offset) {
  for (int i = 0; i < 4; i++)
      buffer[3 - i + offset] = (number >> (i * 8));
}

void write_float_to_buffer(char *buffer, float number, int offset) {
  memcpy(buffer, &number, sizeof(number));
}

int gc_server_send_data(String command) {
  if(serial_debug) {
      Serial.println("gc_server_send_data");
  }

  // write device id
  write_int_to_buffer(send_buffer, 42, 0);
  // write protocol version
  write_int_to_buffer(send_buffer, 100, 4);
  // write realtime mode
  write_int_to_buffer(send_buffer, 1, 8);
  gc_client.write((const uint8_t *) send_buffer, 12);
  gc_client.flush();

  return 0;
}

int gc_server_send_data_point(String command) {
  if(serial_debug) {
      Serial.println("gc_server_send_data_point");
  }

  // write emg value
  write_float_to_buffer(send_buffer, 0.42, 0);

  gc_client.write((const uint8_t *) send_buffer, 4);
  gc_client.flush();

  return 0;
}

void loop() {

  /*
  battery_voltage = lipo.getVoltage();
  battery_soc = lipo.getSOC();

  if(serial_debug) {
    Serial.print("Voltage: ");
  	Serial.print(battery_voltage);  // Print the battery voltage
  	Serial.println(" V");

  	Serial.print("Percentage: ");
  	Serial.print(battery_soc); // Print the battery state of charge
  	Serial.println(" %");
  	Serial.println();
  }


  delay(1000);
  */

  delay(250);

}
