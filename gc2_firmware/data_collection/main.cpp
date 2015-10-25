
// purpose: collect data and upload to gc2 server at 5mn intervals

#include "SparkFunMAX17043.h" // Include the SparkFun MAX17043 library, battery gauge
#include "SparkFunLSM9DS1.h" // include Sparkfun LSM9DS1 library, IMU
#include "math.h"

LSM9DS1 imu;
#define LSM9DS1_M	0x1E
#define LSM9DS1_AG	0x6B


double battery_voltage = 0;
double battery_soc = 0;
bool serial_debug = true;
bool realtime_send = false;

TCPClient gc_client;

char send_buffer[500];

// declare functions
int gc_server_connect(String command);
int gc_server_disconnect(String command);
int gc_server_send_data(String command);
int enable_realtime_send(String command);

void setup() {
  pinMode(A0, INPUT);

  if (serial_debug) {
    Serial.begin(9600);
  }


  // IMU setup
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;

  if (!imu.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
  }


  Particle.function("gc_conn", gc_server_connect);
  Particle.function("gc_disc", gc_server_disconnect);
  Particle.function("gc_send_1", gc_server_send_data);
  Particle.function("realtime", enable_realtime_send);


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

float get_gyro_max() {
  // retrieve the highest gyro rate on 3 axes
  imu.readGyro();
  float gyro_x = imu.calcGyro(imu.gx);
  float gyro_y = imu.calcGyro(imu.gy);
  float gyro_z = imu.calcGyro(imu.gz);

  return max(max(gyro_x, gyro_y), gyro_z);
}

void get_accel(float *accel_values) {
  imu.readAccel();
  accel_values[0] = imu.calcAccel(imu.ax);
  accel_values[1] = imu.calcAccel(imu.ay);
  accel_values[2] = imu.calcAccel(imu.az);
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

  float gyro_max = get_gyro_max();
  float accel_values[3];
  get_accel(accel_values);

  float accel_x = accel_values[0];
  float accel_y = accel_values[1];
  float accel_z = accel_values[2];

  write_float_to_buffer(send_buffer, gyro_max, offset);
  write_float_to_buffer(send_buffer, accel_x, offset);
  write_float_to_buffer(send_buffer, accel_y, offset);
  write_float_to_buffer(send_buffer, accel_z, offset);

}

int enable_realtime_send(String command) {
  if(command == "0") {
    realtime_send = false;
  } else {
    realtime_send = true;
  }

  return 0;
}


int gc_server_send_data_point() {
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

  if(realtime_send) {
    gc_server_send_data_point();
  }

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
