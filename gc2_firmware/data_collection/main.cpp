
// purpose: collect data and upload to gc2 server at 5mn intervals

#include "gc_client.h"
#include "gc_data.h"
#include "common.h"

#define FIRMWARE_VERSION 1

GcClient gc_client;
GcData gc_data;

int firmware_version = FIRMWARE_VERSION;

#define DATA_TRANSFER_DELAY 500
#define COLLECT_DATA_FREQUENCY 250      // 250ms

bool serial_debug = true;

// declare functions
int set_device_id(String command);
int set_mode(String command);
int device_util(String command);

void serial_log(const char *func, int line, String message) {
  Serial.printlnf("%s %s:%d %s", Time.timeStr().c_str(), func, line, message.c_str());
}

int set_device_id(String command) {
  uint32_t device_id = command.toInt();
  gc_client.set_device_id(device_id);
  return 0;
}

void validation_tone() {
  tone(A4, 600, 150);
  delay(150);
  tone(A4, 900, 150);
}


int device_util(String command) {
  if(command=="test_serial") {
    DEBUG_LOG("serial test");
    return 0;
  } else if(command=="setup_serial") {
    Serial.begin(9600);
    return 0;
  } else if(command=="test_tone") {
    tone(A4, 600, 500);
  } else if(command.startsWith("cfg=")) {
    // format: cfg=dev2.photozzap.com,7001,2341234
    int i = command.indexOf("=");
    int j = command.indexOf(",");
    int k = command.indexOf(",", j+1);

    if(i == -1 || j == -1 || k == -1) {
      DEBUG_LOG("config parsing error: " +  command);
    }
    // DEBUG_LOG(String(i) + " " + String(j) + " " + String(k) + " " + String(l));

    // substring between i and j is hostname
    String hostname = command.substring(i+1,j);
    String port = command.substring(j+1,k);
    String deviceId = command.substring(k+1);

    DEBUG_LOG("config: hostname: " + hostname + " port: " + port + " deviceId: " + deviceId);
    validation_tone();

  }


  return 0;
}

void setup() {

  if (serial_debug) {
    Serial.begin(9600);
    // DEBUG_LOG("hit Enter to continue");
    // while(!Serial.available()) Particle.process();
    DEBUG_LOG("serial setup OK");
  }

  gc_data.init(&gc_client);

  Particle.function("device_id", set_device_id);
  Particle.function("device_util", device_util);
  Particle.function("set_mode", set_mode);

  Particle.variable("gc_version", firmware_version);

  gc_client.configure("dev2.photozzap.com", 7001, 42);

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
    gc_client.set_mode(GC_MODE_BATCH);
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
  gc_data.collect_data();
  delay(COLLECT_DATA_FREQUENCY);
}
