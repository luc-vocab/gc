
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
  pinMode(A0, INPUT);

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

void write_int_to_buffer(char *buffer, int number, int *offset) {
  /*
  for (int i = 0; i < 4; i++)
      buffer[3 - i + *offset] = (number >> (i * 8));
  */
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void write_float_to_buffer(char *buffer, float number, int *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void write_int16_to_buffer(char *buffer, uint16_t number, int *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void write_long_to_buffer(char *buffer, unsigned long number, int *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

int gc_server_send_data(String command) {
  if(serial_debug) {
      Serial.println("gc_server_send_data");
  }

  int offset = 0;

  // write device id
  uint32_t device_id = 42;
  write_int_to_buffer(send_buffer, device_id, &offset);
  // write protocol version
  uint32_t protocol_version = 100;
  write_int_to_buffer(send_buffer, protocol_version, &offset);
  // write realtime mode
  uint32_t mode = 1;
  write_int_to_buffer(send_buffer, mode, &offset);

  if(serial_debug) {
      Serial.println("sending data: " + String(offset));
  }


  gc_client.write((const uint8_t *) send_buffer, offset);
  gc_client.flush();

  return 0;
}

void collect_datapoint_append_buffer(int *offset) {
  // collect a data point of each type and write to the buffer

  // write timestamp in two parts, program startup time and millis
  uint32_t timestamp = Time.now();
  write_int_to_buffer(send_buffer, timestamp, offset);
  uint16_t milliseconds = millis() % 1000;
  write_int16_to_buffer(send_buffer, milliseconds, offset);

  // get emg value
  uint16_t emg_value = analogRead(A0);
  write_int16_to_buffer(send_buffer, emg_value, offset);

  float gyro_max = 0.0001;
  float accel_x = 0.001;
  float accel_y = 0.002;
  float accel_z = 0.003;

  write_float_to_buffer(send_buffer, gyro_max, offset);
  write_float_to_buffer(send_buffer, accel_x, offset);
  write_float_to_buffer(send_buffer, accel_y, offset);
  write_float_to_buffer(send_buffer, accel_z, offset);

}


int gc_server_send_data_point(String command) {
  if(serial_debug) {
      Serial.println("gc_server_send_data_point");
  }

  // write data to buffer
  int offset = 0;
  collect_datapoint_append_buffer(&offset);

  if(serial_debug) {
      Serial.println("sending bytes of data: " + String(offset));
  }

  gc_client.write((const uint8_t *) send_buffer, offset);
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
