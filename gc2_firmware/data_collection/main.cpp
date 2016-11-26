
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
#define REALTIME_FREQUENCY 300
#define COLLECT_DATA_FREQUENCY 100

bool serial_debug = true;
bool pending_night_mode = false;
bool upload_requested = false;
int frequency = COLLECT_DATA_FREQUENCY;

bool s_button1_pressed = false;
bool s_button2_pressed = false;


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
  gc_data.queue_status_battery_charge();
  return 0;
}

int device_util(String command) {
  DEBUG_LOG("got command " + command);

  if(command=="test_serial") {
    DEBUG_LOG("serial test");
    return 0;
  } else if(command=="setup_serial") {
    Serial.begin(9600);
    return 0;
  } else if(command=="test_tone") {
    tone(BUZZER_PIN, 600, 500);
  } else if(command=="sim_on") {
    gc_data.set_simulation_mode(true);
  } else if (command=="sim_off") {
    gc_data.set_simulation_mode(false);
  } else if (command=="emg_beep_on") {
    DEBUG_LOG("EMG beep on");
    gc_data.set_emg_beep(true);
  } else if (command=="emg_beep_off") {
    DEBUG_LOG("EMG beep off");
    gc_data.set_emg_beep(false);
  } else {
    DEBUG_LOG("unknown command");
  }

  return 0;
}

int connection_test(String command) {
  validation_tone();
  int random_number = command.toInt();
  gc_client.connection_test(random_number);
  return 0;
}


void button1_pressed()
{
  //s_report_button1_state = true;
}

void button2_pressed()
{
  //s_report_button2_state = true;
  DEBUG_LOG("button 2 on");
  s_button2_pressed = true;
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

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  //attachInterrupt(BUTTON1_PIN, button1_on, FALLING);
  //attachInterrupt(BUTTON2_PIN, button2_on, FALLING);
  attachInterrupt(BUTTON1_PIN, button1_pressed, RISING);
  attachInterrupt(BUTTON2_PIN, button2_pressed, RISING);

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
    frequency = COLLECT_DATA_FREQUENCY;
    DEBUG_LOG("enable batch mode");
    if(MANAGE_WIFI) {
      DEBUG_LOG("turning off wifi");
      WiFi.off();
    }
    pending_night_mode = true;
  } else if (command == "realtime" ) {
    DEBUG_LOG("enable realtime mode");
    frequency = REALTIME_FREQUENCY;
    gc_client.set_mode(GC_MODE_REALTIME);
  } else if (command == "standby") {
    DEBUG_LOG("enable standby mode");
    gc_client.set_mode(GC_MODE_STANDBY);
    gc_data.queue_status_battery_charge();
  }

  return 0;
}

void report_stats() {
  DEBUG_LOG("reporting stats");
  Particle.publish("upload_stats", gc_client.get_stats());
}


void loop() {
  /*
  if(digitalRead(BUTTON1_PIN) == LOW) {
    gc_data.toggle_emg_beep();
    validation_tone();
  }
  */

  if(s_button2_pressed)
  {
    DEBUG_LOG("upload requested");
    upload_requested = true;
    validation_tone();
    s_button2_pressed = false;
  }

  gc_data.collect_data(upload_requested);
  upload_requested = false;
  delay(frequency);
  if (pending_night_mode) {
    if(/*digitalRead(BUTTON2_PIN) == LOW*/ true) {
      DEBUG_LOG("button1 low, starting batch mode");
      // set mode to batch
      pending_night_mode = false;
      validation_tone();
      gc_client.set_mode(GC_MODE_BATCH);
    }
  }
}
