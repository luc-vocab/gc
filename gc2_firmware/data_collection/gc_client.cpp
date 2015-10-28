#include "gc_client.h"
#include "common.h"


GcClient::GcClient() : m_mode(GC_MODE_STANDBY),
                       m_data_buffer_offset(BUFFER_HEADER_LENGTH),
                       m_device_id(0),
                       m_battery_charge(0),
                       m_error_count(0),
                       m_abandon_count(0),
                       m_data_size(0),
                       m_num_datapoints(0){

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
}

void GcClient::set_mode(uint16_t mode) {
  m_mode = mode;
  if(mode == GC_MODE_BATCH) {
    // clear buffer
    reset_data_buffer();
    if(MANAGE_WIFI) {
      DEBUG_LOG("turning off wifi");
      WiFi.off();
    }
  }
}

void GcClient::battery_charge(float percent_charged) {
  m_battery_charge = percent_charged;
}


void GcClient::upload_batch() {
  // try calling upload_batch_iteration up to 3 times

  int i;

  i = BATCH_NUM_RETRIES;
  while(upload_batch_iteration() != SUCCESS_RETURN && i > 0) {
    DEBUG_LOG("FAILED upload_batch_iteration(), retrying " + String(i));
    m_error_count++;
    delay(RETRY_DELAY);
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

  // turn on WiFi
  if(MANAGE_WIFI) {
    DEBUG_LOG("enabling wifi");
    WiFi.on();
  }
  // wait for wifi to be available
  if(!waitFor(WiFi.ready, WIFI_MAX_WAIT)) {
    return ERROR_WIFI_UNAVAILABLE;
  }

  // try calling connect_and_transfer_batch up to 3 times
  rv = connect_and_transfer_batch();
  if (rv != SUCCESS_RETURN) {
    DEBUG_LOG("FAILED connect_and_transfer_batch");
    return rv;
  }

  // turn off wifi
  if(MANAGE_WIFI) {
      DEBUG_LOG("disabling wifi");
      WiFi.off();
  }

  return 0;
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

  rv = initial_handshake(GC_MODE_BATCH);
  if(rv == -1) {
    DEBUG_LOG("ERROR: initial handshake failed");
    return ERROR_TCP_WRITE_FAILED;
  }

  // write data header
  size_t temp_offset = 0;
  // write battery charge
  uint16_t percent_charged = m_battery_charge * 100.0;
  write_int16_to_buffer(m_data_buffer, percent_charged, &temp_offset);
  // indicate how many datapoints we're sending
  write_int16_to_buffer(m_data_buffer, m_num_datapoints, &temp_offset);
  // indicate total buffer size
  write_int_to_buffer(m_data_buffer, m_data_buffer_offset, &temp_offset);

  size_t written_so_far = 0;
  int bytes_written;
  size_t need_to_write;
  size_t message_size;
  while(written_so_far < m_data_buffer_offset) {
    need_to_write = m_data_buffer_offset - written_so_far;
    message_size = min(need_to_write, CHUNK_SIZE);
    DEBUG_LOG("need_to_write: " + String(need_to_write) + " message_size: " + String(message_size));
    bytes_written = m_tcp_client.write((const uint8_t *) m_data_buffer + written_so_far, min(need_to_write, CHUNK_SIZE));
    if(bytes_written == -1) {
      return ERROR_TCP_WRITE_FAILED;
    }
    written_so_far += bytes_written;
    delay(CHUNK_DELAY);
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

  DEBUG_LOG("end");

  return SUCCESS_RETURN;
}

int GcClient::initial_handshake(uint32_t mode) {
  DEBUG_LOG("GcClient::initial_handshake");

  size_t offset = 0;

  // write device id
  write_int_to_buffer(m_handshake_buffer, m_device_id, &offset);
  // write protocol version
  write_int_to_buffer(m_handshake_buffer, PROTOCOL_VERSION, &offset);
  // write mode (batch or realtime)
  write_int_to_buffer(m_handshake_buffer, mode, &offset);

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

  if(m_mode == GC_MODE_BATCH) {
    // check whether we have enough room to write datapoint
    write_datapoint(emg_value, gyro_max, accel_x, accel_y, accel_z);
    if(m_data_buffer_offset + m_data_size > DATA_BUFFER_LENGTH) {
      // we have filled up the buffer
      DEBUG_LOG("Filled up buffer, offset: " + String(m_data_buffer_offset) +
                " datapoints: " + String(m_num_datapoints));
      upload_batch();
    }
  }

}

void GcClient::write_datapoint(uint16_t emg_value, float gyro_max, float accel_x, float accel_y, float accel_z) {
  size_t original_offset = m_data_buffer_offset;

  size_t *offset = &m_data_buffer_offset;

  // write timestamp in two parts, program startup time and millis
  uint32_t timestamp = Time.now();
  write_int_to_buffer(m_data_buffer, timestamp, offset);
  uint16_t milliseconds = millis() % 1000;
  write_int16_to_buffer(m_data_buffer, milliseconds, offset);

  write_int16_to_buffer(m_data_buffer, emg_value, offset);

  write_float_to_buffer(m_data_buffer, gyro_max, offset);
  write_float_to_buffer(m_data_buffer, accel_x, offset);
  write_float_to_buffer(m_data_buffer, accel_y, offset);
  write_float_to_buffer(m_data_buffer, accel_z, offset);

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

void write_int16_to_buffer(char *buffer, uint16_t number, size_t *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}

void write_long_to_buffer(char *buffer, unsigned long number, size_t *offset) {
  memcpy(buffer + *offset, &number, sizeof(number));
  *offset += sizeof(number);
}
