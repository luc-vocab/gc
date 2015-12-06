#include "gc_client.h"
#include "common.h"


GcClient::GcClient() : m_mode(GC_MODE_STANDBY),
                       m_data_buffer_offset(BUFFER_HEADER_LENGTH),
                       m_device_id(0),
                       m_battery_charge(0),
                       m_error_count(0),
                       m_abandon_count(0),
                       m_batch_upload_count(0),
                       m_data_size(0),
                       m_num_datapoints(0),
                       m_start_timestamp(0),
                       m_start_millis(0),
                       m_data_collection_start_timestamp(0){

}

int GcClient::connect() {
  DEBUG_LOG("connecting to " + m_host + ":" + String(m_port));
  return m_tcp_client.connect(m_host, m_port);
}

void GcClient::disconnect() {
  DEBUG_LOG("disconnecting from " + m_host + ":" + String(m_port));
  m_tcp_client.stop();
}

void GcClient::configure(String host, int port, uint32_t device_id) {
  m_host = host;
  m_port = port;
  m_device_id = device_id;

  DEBUG_LOG("set config: " + host + " port: " + String(port) + " device_id: " + String(device_id));
}

void GcClient::set_mode(uint16_t mode) {
  m_mode = mode;
  if(mode == GC_MODE_BATCH) {
    m_data_collection_start_timestamp = Time.now();
    // clear buffer
    reset_data_buffer();
  }

  if(mode == GC_MODE_REALTIME) {
    connect();
    initial_handshake(m_mode, 0);
  }
}

void GcClient::set_device_id(uint32_t device_id) {
  m_device_id = device_id;
  DEBUG_LOG("Set device id to " + String(m_device_id));
}

void GcClient::battery_charge(float percent_charged) {
  DEBUG_LOG("received battery_charge: " + String(percent_charged));
  m_battery_charge = percent_charged;
}

String GcClient::get_stats() {
  return "batches: " + String(m_batch_upload_count) + " errors: " + String(m_error_count) + " abandon: " + String(m_abandon_count);
}

void GcClient::upload_batch() {
  // try calling upload_batch_iteration up to 3 times

  int i;

  i = BATCH_NUM_RETRIES;
  while(upload_batch_iteration() != SUCCESS_RETURN && i > 0) {
    DEBUG_LOG("FAILED upload_batch_iteration(), retrying " + String(i));
    m_error_count++;
    delay(LONG_RETRY_DELAY);
    i--;
  }

  if(i == 0) {
    DEBUG_LOG("failed all retries, giving up on uploading batch");
    m_abandon_count++;
    reset_data_buffer();
  }

  DEBUG_LOG(String::format("errors: %d abandonned: %d", m_error_count, m_abandon_count));
}

int GcClient::upload_batch_iteration() {
  int i;
  int rv;

  // try calling connect_and_transfer_batch up to 3 times
  rv = connect_and_transfer_batch();
  if (rv != SUCCESS_RETURN) {
    DEBUG_LOG("FAILED connect_and_transfer_batch");
    return rv;
  }

  return 0;
}

void GcClient::connection_test(int random_number) {
  DEBUG_LOG("running connection test with random_number: " + String(random_number));
  GcClient::connect();
  initial_handshake(GC_MODE_CONNECTION_TEST, random_number);
  m_tcp_client.stop();
}

int GcClient::connect_and_transfer_batch() {
  int i;
  int rv;

  DEBUG_LOG("begin");

  rv = GcClient::connect();
  if( !rv ) {
    DEBUG_LOG("ERROR: TCP connection failed");
    return ERROR_TCP_CONNECTION_FAILED;
  }

  if( ! m_tcp_client.status() ) {
    DEBUG_LOG("ERROR: not connected");
    return ERROR_TCP_CONNECTION_FAILED;
  }

  rv = initial_handshake(GC_MODE_BATCH, 0);
  if(rv == -1) {
    DEBUG_LOG("ERROR: initial handshake failed");
    return ERROR_TCP_WRITE_FAILED;
  }

  delay(TRANSFER_DELAY);

  // write data header
  size_t temp_offset = 0;
  // write battery charge
  uint16_t percent_charged = m_battery_charge * 100.0;
  write_uint16_to_buffer(m_data_buffer, percent_charged, &temp_offset);
  // write number of seconds collected
  write_int_to_buffer(m_data_buffer, Time.now() - m_data_collection_start_timestamp, &temp_offset );
  // write starting timestamp
  write_int_to_buffer(m_data_buffer, m_start_timestamp, &temp_offset );
  // write starting millis
  write_int_to_buffer(m_data_buffer, m_start_millis, &temp_offset);
  // write how many batches we've uploaded so far
  write_uint16_to_buffer(m_data_buffer, m_batch_upload_count, &temp_offset);
  // write how many errors so far
  write_uint16_to_buffer(m_data_buffer, m_error_count, &temp_offset);
  // write how many batches were abandonned so far
  write_uint16_to_buffer(m_data_buffer, m_abandon_count, &temp_offset);
  // indicate how many datapoints we're sending
  write_uint16_to_buffer(m_data_buffer, m_num_datapoints, &temp_offset);
  // indicate total buffer size
  write_int_to_buffer(m_data_buffer, m_data_buffer_offset + END_MARKER_LENGTH, &temp_offset);
  // add starting marker
  write_uint16_to_buffer(m_data_buffer, UINT16_MARKER_START, &temp_offset);
  // add ending marker
  temp_offset = m_data_buffer_offset;
  write_uint16_to_buffer(m_data_buffer, UINT16_MARKER_END, &temp_offset);

  delay(INITIAL_DELAY);

  size_t written_so_far = 0;
  int bytes_written;
  size_t need_to_write;
  size_t message_size;
  i = 0;
  while(written_so_far < m_data_buffer_offset + END_MARKER_LENGTH) {
    need_to_write = m_data_buffer_offset + END_MARKER_LENGTH - written_so_far;
    message_size = min(need_to_write, CHUNK_SIZE);
    DEBUG_LOG("need_to_write: " + String(need_to_write) + " message_size: " + String(message_size));
    bytes_written = m_tcp_client.write((const uint8_t *) m_data_buffer + written_so_far, min(need_to_write, CHUNK_SIZE));
    if(bytes_written == -1) {
      return ERROR_TCP_WRITE_FAILED;
    }
    written_so_far += bytes_written;
    delay(CHUNK_DELAY);
    i++;
  }



  i = 8;
  while( m_tcp_client.read() == -1 && i > 0) {
    delay(RETRY_DELAY);
    DEBUG_LOG("waiting for final byte sent by server");
    i--;
  }
  if(i == 0) {
    DEBUG_LOG("ERROR: didn't receive final byte confirmation");
    return ERROR_NO_FINAL_CONFIRMATION;
  }

  m_tcp_client.stop();

  reset_data_buffer();

  m_batch_upload_count++;

  DEBUG_LOG("end");

  return SUCCESS_RETURN;
}

int GcClient::initial_handshake(uint32_t mode, int random_number) {
  DEBUG_LOG("GcClient::initial_handshake");

  size_t offset = 0;

  // write device id
  write_int_to_buffer(m_handshake_buffer, m_device_id, &offset);
  // write protocol version
  write_int_to_buffer(m_handshake_buffer, PROTOCOL_VERSION, &offset);
  // write mode (batch or realtime)
  write_int_to_buffer(m_handshake_buffer, mode, &offset);

  if(mode == GC_MODE_CONNECTION_TEST) {
    // add random number used for connection test
    write_int_to_buffer(m_handshake_buffer, random_number, &offset);
  }

  int rv = m_tcp_client.write((const uint8_t *) m_handshake_buffer, offset);
  m_tcp_client.flush();

  DEBUG_LOG("GcClient::initial_handshake done");
  return rv;
}

void GcClient::add_datapoint(uint16_t emg_value, float gyro_max, float accel_x, float accel_y, float accel_z) {

  if(m_mode == GC_MODE_STANDBY) {
    // discard the data
    return;
  }

  if (m_mode == GC_MODE_BATCH) {
    write_datapoint(emg_value, gyro_max, accel_x, accel_y, accel_z);
  }

  if (m_mode == GC_MODE_REALTIME) {
    // write at the beginning of the buffer
    m_data_buffer_offset = 0;
    write_datapoint(emg_value, gyro_max, accel_x, accel_y, accel_z);
    // immediately send
    m_tcp_client.write((const uint8_t *) m_data_buffer, m_data_buffer_offset);
  }


}

bool GcClient::need_upload() {
  if(m_data_buffer_offset + m_data_size + END_MARKER_LENGTH > DATA_BUFFER_LENGTH) {
    return true;
  }
  return false;
}

void GcClient::write_datapoint(uint16_t emg_value, float gyro_max, float accel_x, float accel_y, float accel_z) {
  size_t original_offset = m_data_buffer_offset;

  size_t *offset = &m_data_buffer_offset;

  // write timestamp in two parts, program startup time and millis
  uint32_t milliseconds = millis();
  write_int_to_buffer(m_data_buffer, milliseconds, offset);

  write_uint16_to_buffer(m_data_buffer, emg_value, offset);

  int16_t gyro_value = gyro_max * 100.0;
  write_int16_to_buffer(m_data_buffer, gyro_value, offset);

  int16_t accel_x_value = accel_x * 10000.0;
  int16_t accel_y_value = accel_y * 10000.0;
  int16_t accel_z_value = accel_z * 10000.0;

  write_int16_to_buffer(m_data_buffer, accel_x_value, offset);
  write_int16_to_buffer(m_data_buffer, accel_y_value, offset);
  write_int16_to_buffer(m_data_buffer, accel_z_value, offset);

  m_data_size = m_data_buffer_offset - original_offset;

  m_num_datapoints++;
  if(m_num_datapoints % 10 == 0) {
    DEBUG_LOG("num datapoints: " + String(m_num_datapoints) +
              " offset: " + String(m_data_buffer_offset) + " datasize: " + String(m_data_size));
  }
}

void GcClient::reset_data_buffer() {
  DEBUG_LOG("GcClient::reset_data_buffer");
  m_data_buffer_offset = BUFFER_HEADER_LENGTH;
  m_num_datapoints = 0;
  m_start_timestamp = Time.now();
  m_start_millis = millis();
}

// non-member functions
void write_int_to_buffer(char *buffer, int number, size_t *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void write_float_to_buffer(char *buffer, float number, size_t *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void write_uint16_to_buffer(char *buffer, uint16_t number, size_t *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void write_int16_to_buffer(char *buffer, int16_t number, size_t *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void write_long_to_buffer(char *buffer, unsigned long number, size_t *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}
