
// purpose: collect data and upload to gc2 server at 5mn intervals

#include "gc_client.h"
#include "gc_data.h"
#include "gc_config.h"
#include "common.h"
#include "utils.h"

#define FIRMWARE_VERSION 1

GcClient gc_client;
GcData gc_data(gc_client);
GcConfig gc_config(gc_client);

// cloud variables
int firmware_version = FIRMWARE_VERSION;

#define DATA_TRANSFER_DELAY 500
#define COLLECT_DATA_FREQUENCY 100      // 250ms

bool serial_debug = true;
bool pending_night_mode = false;
bool upload_requested = false;

// declare functions
int set_device_id(String command);
int set_mode(String command);
int device_util(String command);
int connection_test(String command);

void serial_log(const char *func, int line, String message) {
  Serial.printlnf("%s %s:%d %s", Time.timeStr().c_str(), func, line, message.c_str());
}

int set_config(String command) {
  gc_config.set_config(command);
  return 0;
}

int device_util(String command) {
  if(command=="test_serial") {
    DEBUG_LOG("serial test");
    return 0;
  } else if(command=="setup_serial") {
    Serial.begin(9600);
    return 0;
  } else if(command=="test_tone") {
    tone(BUZZER_PIN, 600, 500);
  }

  return 0;
}

int connection_test(String command) {
  validation_tone();
  int random_number = command.toInt();
  gc_client.connection_test(random_number);
  return 0;
}

void setup() {

  if (serial_debug) {
    Serial.begin(9600);
    // DEBUG_LOG("hit Enter to continue");
    // while(!Serial.available()) Particle.process();
    DEBUG_LOG("serial setup OK");
  }

  gc_data.init();
  gc_config.init();

  Particle.function("device_util", device_util);
  Particle.function("set_mode", set_mode);
  Particle.function("set_config", set_config);
  Particle.function("conn_test", connection_test);

  Particle.variable("gc_version", firmware_version);
  Particle.variable("gc_hostname", gc_config.p_hostname);
  Particle.variable("gc_port", gc_config.p_port);
  Particle.variable("gc_device_id", gc_config.p_device_id);
  Particle.variable("setup_done", gc_config.p_setup_done);
  Particle.variable("bat_charge", gc_data.p_battery_charge);

  DEBUG_LOG("started up");

  validation_tone();

}

int set_mode(String command) {
  if(command == "batch") {
    DEBUG_LOG("enable batch mode");
    if(MANAGE_WIFI) {
      DEBUG_LOG("turning off wifi");
      WiFi.off();
    }
    pending_night_mode = true;
  } else if (command == "realtime" ) {
    DEBUG_LOG("enable realtime mode");
    gc_client.set_mode(GC_MODE_REALTIME);
  } else if (command == "standby") {
    DEBUG_LOG("enable standby mode");
    gc_client.set_mode(GC_MODE_STANDBY);
  }

  return 0;
}

void report_stats() {
  DEBUG_LOG("reporting stats");
  Particle.publish("upload_stats", gc_client.get_stats());
}


void loop() {
  if(digitalRead(BUTTON2_PIN) == LOW) {
    upload_requested = true;
    validation_tone();
  }
  gc_data.collect_data(upload_requested);
  upload_requested = false;
  delay(COLLECT_DATA_FREQUENCY);
  if (pending_night_mode) {
    if(digitalRead(BUTTON1_PIN) == LOW) {
      DEBUG_LOG("button1 low, starting batch mode");
      // set mode to batch
      pending_night_mode = false;
      validation_tone();
      gc_client.set_mode(GC_MODE_BATCH);
    }
  }
}
