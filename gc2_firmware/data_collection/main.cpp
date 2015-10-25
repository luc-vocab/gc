
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
bool batch_send = false;

TCPClient gc_client;

#define MANAGE_WIFI false // whether to switch off wifi in batch mode

#define DEBUG_LOG(x) serial_log(x)

#define HANDSHAKE_BUFFER_LENGTH 64
#define BUFFER_HEADER_LENGTH 8
#define BUFFER_LENGTH 28808
#define CHUNK_SIZE 1024
char handshake_buffer[HANDSHAKE_BUFFER_LENGTH];
char send_buffer[BUFFER_LENGTH];
uint32_t current_offset = BUFFER_HEADER_LENGTH;
uint16_t num_datapoints = 0;

#define GC_MODE_REALTIME 1
#define GC_MODE_BATCH 2

// declare functions
int gc_server_connect(String command);
int gc_server_disconnect(String command);
int gc_server_send_data(String command);
int set_mode(String command);
int device_util(String command);


void reset_batch_buffer();

void serial_log(String message) {
  if(serial_debug){
    Serial.println(message);
  }
}

int device_util(String command) {
  if(command=="test_serial") {
    DEBUG_LOG("serial test");
    return 0;
  }

  if(command=="setup_serial") {
    Serial.begin(9600);
    return 0;
  }
}

void setup() {
  pinMode(A0, INPUT);

  if (serial_debug) {
    Serial.begin(9600);
    // DEBUG_LOG("hit Enter to continue");
    // while(!Serial.available()) Particle.process();
    DEBUG_LOG("serial setup OK");
  }


  // IMU setup
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;

  if (!imu.begin())
  {
    DEBUG_LOG("Failed to communicate with LSM9DS1.");
  }


  Particle.function("gc_conn", gc_server_connect);
  Particle.function("gc_disc", gc_server_disconnect);
  Particle.function("device_util", device_util);
  Particle.function("set_mode", set_mode);


  // setup battery gauge
  lipo.begin();
  lipo.quickStart();

  Particle.publish("startup");

  DEBUG_LOG("started up");

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

void write_int_to_buffer(char *buffer, int number, uint32_t *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void write_float_to_buffer(char *buffer, float number, uint32_t *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void write_int16_to_buffer(char *buffer, uint16_t number, uint32_t *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void write_long_to_buffer(char *buffer, unsigned long number, uint32_t *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void gc_server_initial_handshake(uint32_t mode) {
  DEBUG_LOG("gc_server_initial_handshake");

  uint32_t offset = 0;

  // write device id
  uint32_t device_id = 42;
  write_int_to_buffer(handshake_buffer, device_id, &offset);
  // write protocol version
  uint32_t protocol_version = 100;
  write_int_to_buffer(handshake_buffer, protocol_version, &offset);
  // write mode (batch or realtime)
  write_int_to_buffer(handshake_buffer, mode, &offset);

  gc_client.write((const uint8_t *) handshake_buffer, offset);
  gc_client.flush();

  DEBUG_LOG("gc_server_initial_handshake done");
}

int gc_server_send_data(String command) {

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

void collect_datapoint_append_buffer(uint32_t *offset) {
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

int set_mode(String command) {
  if(command == "batch") {
    DEBUG_LOG("enabled batch mode");

    realtime_send = false;
    batch_send = true;

    if(MANAGE_WIFI)
      WiFi.off();

    reset_batch_buffer();

  } else if(command == "realtime") {
    DEBUG_LOG("enabling realtime mode");
    realtime_send = true;
    batch_send = false;
  } else {
    DEBUG_LOG("disabling realtime and batch mode");
    realtime_send = false;
    batch_send = false;
  }

  return 0;
}


int gc_server_send_data_point() {
  if(serial_debug) {
      Serial.println("gc_server_send_data_point");
  }

  // write data to buffer
  uint32_t offset = 0;
  collect_datapoint_append_buffer(&offset);

  if(serial_debug) {
      Serial.println("sending bytes of data: " + String(offset));
  }

  gc_client.write((const uint8_t *) send_buffer, offset);
  gc_client.flush();

  return 0;
}

void reset_batch_buffer() {
  current_offset = BUFFER_HEADER_LENGTH;
  num_datapoints = 0;
}

int gc_server_upload_batch() {
  DEBUG_LOG("gc_server_upload_batch begin");

  if(MANAGE_WIFI)
    WiFi.on();

  while(! WiFi.ready()) {
    if(serial_debug) {
        Serial.println("waiting for wifi to be available");
    }
    delay(250);
  }

  // connect to GC server
  if( ! gc_client.connect("dev2.photozzap.com", 7001) ) {
    DEBUG_LOG("TCPClient connection failed");
    return -1;
  }
  // indicate that we're uploading a batch
  gc_server_initial_handshake(GC_MODE_BATCH);



  // write data header
  uint32_t temp_offset = 0;
  // write battery charge
  uint16_t percent_charged = lipo.getSOC() * 100.0;
  write_int16_to_buffer(send_buffer, percent_charged, &temp_offset);
  // indicate how many datapoints we're sending
  write_int16_to_buffer(send_buffer, num_datapoints, &temp_offset);
  // indicate total buffer size
  write_int_to_buffer(send_buffer, current_offset, &temp_offset);

  uint32_t written_so_far = 0;
  uint32_t bytes_written;
  uint32_t need_to_write;
  uint32_t message_size;
  while(written_so_far < current_offset) {
    need_to_write = current_offset - written_so_far;
    message_size = min(need_to_write, CHUNK_SIZE);
    DEBUG_LOG("need_to_write: " + String(need_to_write) + " message_size: " + String(message_size));
    bytes_written = gc_client.write((const uint8_t *) send_buffer + written_so_far, min(need_to_write, CHUNK_SIZE));
    written_so_far += bytes_written;
    if(bytes_written != message_size) {
      DEBUG_LOG("wrote less than expected");
    }
  }
  gc_client.flush();

  reset_batch_buffer();

  // wait for server to send us final byte, indicating data has been received
  while( gc_client.read() == -1 ) {
    delay(100);
    DEBUG_LOG("waiting for final byte sent by server");
  }


  // disconnect from GC server
  gc_client.stop();

  if(MANAGE_WIFI)
    WiFi.off();

  DEBUG_LOG("gc_server_upload_batch end");
}


void loop() {

  if(batch_send) {
    int previous_offset = current_offset;
    collect_datapoint_append_buffer(&current_offset);
    num_datapoints++;
    int data_size = current_offset - previous_offset;
    if(current_offset + data_size > BUFFER_LENGTH) {
      // time to send buffer to GC server
      if(serial_debug) {
        Serial.println("filled up buffer, offset: " + String(current_offset) +
                       " datapoints: " + String(num_datapoints));
      }
      gc_server_upload_batch();
    }
  }

  if(realtime_send) {
    gc_server_send_data_point();
  }

  delay(250);

}
