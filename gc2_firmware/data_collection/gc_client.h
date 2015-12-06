#ifndef _GC_CLIENT_H
#define _GC_CLIENT_H

#include "application.h"

#define PROTOCOL_VERSION 100

#define GC_MODE_STANDBY 0
#define GC_MODE_REALTIME 1
#define GC_MODE_BATCH 2
#define GC_MODE_CONNECTION_TEST 3

#define GC_BUFFER_LENGTH 64
#define HANDSHAKE_BUFFER_LENGTH 64
#define BUFFER_HEADER_LENGTH 28
#define END_MARKER_LENGTH 2


//#define DATA_BUFFER_LENGTH 500 + BUFFER_HEADER_LENGTH + END_MARKER_LENGTH // a few seconds of data
//#define DATA_BUFFER_LENGTH 1440 + BUFFER_HEADER_LENGTH + END_MARKER_LENGTH // 15s of data
//#define DATA_BUFFER_LENGTH 5760 + BUFFER_HEADER_LENGTH + END_MARKER_LENGTH // 1mn of data
//#define DATA_BUFFER_LENGTH 10000 + BUFFER_HEADER_LENGTH + END_MARKER_LENGTH // a few minutes of data
//#define DATA_BUFFER_LENGTH 16800  + BUFFER_HEADER_LENGTH + END_MARKER_LENGTH // 5mn of data
//#define DATA_BUFFER_LENGTH 28800 + BUFFER_HEADER_LENGTH + END_MARKER_LENGTH // 5mn of data
#define DATA_BUFFER_LENGTH 50000 + BUFFER_HEADER_LENGTH + END_MARKER_LENGTH // possibly longer
#define CHUNK_SIZE 512
#define CHUNK_DELAY 20 // amount of time to wait between chunks
#define INITIAL_DELAY 3000 // amount of time to wait before sending full buffer of data

#define WIFI_MAX_WAIT 5000
#define TRANSFER_DELAY 250
#define RETRY_DELAY 250
#define LONG_RETRY_DELAY 4000

#define BATCH_NUM_RETRIES 3

#define SUCCESS_RETURN 0
#define ERROR_TCP_CONNECTION_FAILED -1
#define ERROR_TCP_WRITE_FAILED -2
#define ERROR_NO_FINAL_CONFIRMATION -3
#define ERROR_WIFI_UNAVAILABLE -4
#define ERROR_FAILED_RETRIES -5

#define UINT16_MARKER_START 6713 // after header in batch mode
#define UINT16_MARKER_END 21826  // after end of data in batch mode

// functions for serializing data
void write_int_to_buffer(char *buffer, int number, size_t *offset);
void write_float_to_buffer(char *buffer, float number, size_t *offset);
void write_uint16_to_buffer(char *buffer, uint16_t number, size_t *offset);
void write_int16_to_buffer(char *buffer, int16_t number, size_t *offset);
void write_long_to_buffer(char *buffer, unsigned long number, size_t *offset);

class GcClient {

public:
  GcClient();
  void configure(String host, int port, uint32_t device_id);
  void add_datapoint(uint16_t emg_value, float gyro_max, float accel_x, float accel_y, float accel_z);
  void battery_charge(float percent_charged);
  void set_mode(uint16_t mode);
  void set_device_id(uint32_t device_id);
  String get_stats();
  // whether data needs to be uploaded
  bool need_upload();
  // upload batch when data buffer is full. Will repeat N times
  void upload_batch();
  // do a connection test to ensure the device can communicate end to end
  void connection_test(int random_number);

private:

  // try to upload batch once
  int upload_batch_iteration();

  // connect using TCPClient and transfer data
  int connect_and_transfer_batch();

  // reset the data buffer
  void reset_data_buffer();

  // write datapoint onto data buffer
  void write_datapoint(uint16_t emg_value, float gyro_max, float accel_x, float accel_y, float accel_z);

  // send initial handshake, identifying the device and specifying what kind of data
  // we'll be sending
  int initial_handshake(uint32_t mode, int random_number);

  // connect to GC server
  int connect();

  // disconnect from GC server
  void disconnect();

  String m_host;
  int m_port;

  // buffer which will be used when initially connecting
  char m_handshake_buffer[HANDSHAKE_BUFFER_LENGTH];

  // size of one measurement
  size_t m_data_size;

  // current offset into the data buffer
  size_t m_data_buffer_offset;

  // data readings will accumulate into this buffer
  char m_data_buffer[DATA_BUFFER_LENGTH];

  // number of data readings written so far
  uint16_t m_num_datapoints;

  // TCPClient
  TCPClient m_tcp_client;

  // unique number identifying the device
  uint32_t m_device_id;

  // starting point for collecting the data
  uint32_t m_data_collection_start_timestamp;
  uint32_t m_start_timestamp;
  uint32_t m_start_millis;

  // percent battery charged
  float m_battery_charge;

  // realtime or batch mode
  uint16_t m_mode;

  // stats and error counters
  uint16_t m_batch_upload_count;
  uint16_t m_error_count;
  uint16_t m_abandon_count;
};


#endif // _GC_CLIENT_H
