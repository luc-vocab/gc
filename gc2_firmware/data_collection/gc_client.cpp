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
                       m_data_collection_start_timestamp(0),
                       m_wifi_on_started(false){

                         for(int i = 0; i<DATAPOINT_HISTORY_SIZE; i++)
                         {
                           m_upload_timestamps[i] = 0;
                         }
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

void GcClient::wifi_on(){
  // turn on WiFi
  if(MANAGE_WIFI && ! m_wifi_on_started) {
    DEBUG_LOG("enabling wifi");
    WiFi.on();
    m_wifi_on_started = true;
  }

}

void GcClient::wifi_off(){
  // turn off wifi
  if(MANAGE_WIFI) {
      DEBUG_LOG("disabling wifi");
      WiFi.off();
      m_wifi_on_started = false;
  }

}

void GcClient::wifi_wait(){

  DEBUG_LOG("wait for wifi to be available");
  // wait for wifi to be available
  waitFor(WiFi.ready, WIFI_MAX_WAIT);
  DEBUG_LOG("wifi available");



}



void GcClient::upload_batch() {
  // try calling upload_batch_iteration up to 3 times

  wifi_on();
  wifi_wait();

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
  wifi_off();

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

void GcClient::report_battery_charge() {
  if( m_mode != GC_MODE_STANDBY) {
    // only do this in standby
    return;
  }
  if( m_host.length() == 0 ) {
    // no server configured
    return;
  }

  DEBUG_LOG("Connecting");

  connect();
  initial_handshake(GC_MODE_REPORT_BATTERY, 0);

  // write data header
  size_t temp_offset = 0;
  // write battery charge
  uint16_t percent_charged = m_battery_charge * 100.0;
  write_uint16_to_buffer(m_data_buffer, percent_charged, &temp_offset);

  // write out buffer
  m_tcp_client.write((const uint8_t *) m_data_buffer, temp_offset);

  // disconnect
  m_tcp_client.stop();

  DEBUG_LOG("Reported battery charge, disconnecting");

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
  // write data collection start timestamp
  write_int_to_buffer(m_data_buffer, m_data_collection_start_timestamp, &temp_offset );
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
  // write marker
  write_uint16_to_buffer(m_handshake_buffer, UINT16_MARKER_HANDSHAKE, &offset);
  // write device id
  write_int_to_buffer(m_handshake_buffer, m_device_id, &offset);
  // write protocol version
  write_int_to_buffer(m_handshake_buffer, PROTOCOL_VERSION, &offset);
  // write mode (batch or realtime)
  write_int_to_buffer(m_handshake_buffer, mode, &offset);
  // write timestamp and millis
  write_int_to_buffer(m_handshake_buffer, Time.now(), &offset);
  write_int_to_buffer(m_handshake_buffer, millis(), &offset);

  if(mode == GC_MODE_CONNECTION_TEST) {
    // add random number used for connection test
    write_int_to_buffer(m_handshake_buffer, random_number, &offset);
  }

  int rv = m_tcp_client.write((const uint8_t *) m_handshake_buffer, offset);
  m_tcp_client.flush();

  // now wait for acknowledgement byte
  int i = 8;
  while( m_tcp_client.read() == -1 && i > 0) {
    delay(RETRY_DELAY);
    DEBUG_LOG("waiting for final byte sent by server");
    i--;
  }
  if(i == 0) {
    DEBUG_LOG("ERROR: didn't receive final byte confirmation");
    return ERROR_NO_FINAL_CONFIRMATION;
  }

  DEBUG_LOG("GcClient::initial_handshake done");
  return rv;
}

void GcClient::add_datapoint(data_point &dp) {

  if(m_mode == GC_MODE_STANDBY) {
    // discard the data
    return;
  }

  if (m_mode == GC_MODE_BATCH) {
    write_datapoint(dp);
  }

  if (m_mode == GC_MODE_REALTIME) {
    // write at the beginning of the buffer
    m_data_buffer_offset = 0;
    write_datapoint(dp);
    // immediately send
    m_tcp_client.write((const uint8_t *) m_data_buffer, m_data_buffer_offset);
  }

}

void GcClient::add_stddev(std_dev &sd)
{

  if(m_mode == GC_MODE_STANDBY) {
    // discard the data
    return;
  }

  if (m_mode == GC_MODE_BATCH) {
    write_stddev(sd);
  }

  if (m_mode == GC_MODE_REALTIME) {
    // write at the beginning of the buffer
    m_data_buffer_offset = 0;
    write_stddev(sd);
    // immediately send
    m_tcp_client.write((const uint8_t *) m_data_buffer, m_data_buffer_offset);
  }

}

uint32_t GcClient::time_remaining()
{
  uint32_t lower = m_upload_timestamps[DATAPOINT_HISTORY_SIZE-1];
  uint32_t upper = millis();
  uint32_t diff = upper - lower;

  uint32_t remaining_dp = datapoints_remaining();
  uint32_t size = DATAPOINT_HISTORY_SIZE;

  uint32_t result = (remaining_dp * diff) / size;

  return result;
}

uint16_t GcClient::datapoints_remaining()
{
  uint16_t space_remaining = DATA_BUFFER_LENGTH - END_MARKER_LENGTH - m_data_buffer_offset;
  uint16_t datapoints_remaining = space_remaining / m_data_size;
  return datapoints_remaining;
}

bool GcClient::need_upload() {
  // assume we need to add both a datapoint and an stddev
  if(m_data_buffer_offset + sizeof(data_point) + sizeof(std_dev) + END_MARKER_LENGTH > DATA_BUFFER_LENGTH) {
    return true;
  }
  return false;
}

bool GcClient::need_upload_soon(){
  return time_remaining() <= 15000;
}

void GcClient::write_stddev(const std_dev &sd)
{
  size_t original_offset = m_data_buffer_offset;

  write_uint8_to_buffer(m_data_buffer + m_data_buffer_offset, DATATYPE_STDDEV, &m_data_buffer_offset);

  memcpy(m_data_buffer + m_data_buffer_offset, &sd, sizeof(std_dev));
  m_data_buffer_offset +=  sizeof(std_dev);

  m_data_size = m_data_buffer_offset - original_offset;
  m_num_datapoints++;
}

void GcClient::write_datapoint(const data_point &dp) {
  size_t original_offset = m_data_buffer_offset;

  write_uint8_to_buffer(m_data_buffer + m_data_buffer_offset, DATATYPE_DATAPOINT, &m_data_buffer_offset);

  memcpy(m_data_buffer + m_data_buffer_offset, &dp, sizeof(data_point));
  m_data_buffer_offset +=  sizeof(data_point);

  m_data_size = m_data_buffer_offset - original_offset;

  m_num_datapoints++;

  update_upload_timestamps(dp.milliseconds);

  if(m_num_datapoints % 10 == 0) {
    uint16_t remaining = datapoints_remaining();
    uint32_t remaining_time =  time_remaining();
    DEBUG_LOG("num datapoints: " + String(m_num_datapoints) +
              " offset: " + String(m_data_buffer_offset) + " datasize: " + String(m_data_size) +
              " datapoints_remaining: " + String(remaining) +
              " time_remaining: " + String(remaining_time));
  }

  if (need_upload_soon()) {
    wifi_on();
  }

}

void GcClient::update_upload_timestamps(uint32_t millis)
{
  // move everything back
  for(int i = DATAPOINT_HISTORY_SIZE - 1; i>=1; i--)
  {
    m_upload_timestamps[i] = m_upload_timestamps[i-1];
  }
  m_upload_timestamps[0] = millis;
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

void write_uint8_to_buffer(char *buffer, uint8_t data, size_t *offset) {
  memcpy(buffer + *offset, &data, sizeof(data));
  *offset += sizeof(data);
}
