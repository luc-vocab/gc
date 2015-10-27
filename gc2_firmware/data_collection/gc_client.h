#ifndef _GC_CLIENT_H
#define _GC_CLIENT_H

#include "application.h"

#define PROTOCOL_VERSION 100

#define GC_MODE_STANDBY 0
#define GC_MODE_REALTIME 1
#define GC_MODE_BATCH 2

#define GC_BUFFER_LENGTH 64
#define HANDSHAKE_BUFFER_LENGTH 64
#define BUFFER_HEADER_LENGTH 8

//#define DATA_BUFFER_LENGTH 5768 // 1mn of data
#define DATA_BUFFER_LENGTH 28808 // 5mn of data
#define CHUNK_SIZE 1024

#define MANAGE_WIFI true // whether to switch off wifi in batch mode

#define WIFI_MAX_WAIT 5000
#define RETRY_DELAY 250

#define BATCH_NUM_RETRIES 3

#define SUCCESS_RETURN 0
#define ERROR_TCP_CONNECTION_FAILED -1
#define ERROR_TCP_WRITE_FAILED -2
#define ERROR_NO_FINAL_CONFIRMATION -3
#define ERROR_WIFI_UNAVAILABLE -4
#define ERROR_FAILED_RETRIES -5

// functions for serializing data
void write_int_to_buffer(char *buffer, int number, size_t *offset);
void write_float_to_buffer(char *buffer, float number, size_t *offset);
void write_int16_to_buffer(char *buffer, uint16_t number, size_t *offset);
void write_long_to_buffer(char *buffer, unsigned long number, size_t *offset);

class GcClient {

public:
  GcClient();
  void configure(String host, int port, uint32_t device_id);
  void add_datapoint(uint16_t emg_value, float gyro_max, float accel_x, float accel_y, float accel_z);
  void battery_charge(float percent_charged);
  void set_mode(uint16_t mode);

private:

  // upload batch when data buffer is full. Will repeat N times
  void upload_batch();

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
  int initial_handshake(uint32_t mode);

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

  // percent battery charged
  float m_battery_charge;

  // realtime or batch mode
  uint16_t m_mode;

  // error counters
  uint16_t m_error_count;
  uint16_t m_abandon_count;
};


#endif // _GC_CLIENT_H
