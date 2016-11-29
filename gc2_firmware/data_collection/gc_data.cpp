#include "gc_data.h"
#include "common.h"
#include "utils.h"

GcData::GcData(GcClient &gc_client) :
    m_gc_client(gc_client),
    #if USE_IMU_1_BNO055
    m_bno_1(-1, BNO055_ADDRESS_A),
    #endif
    #if USE_IMU_2_MMA8452
    m_mma_2(),
    #endif
    p_battery_charge(0),
    m_last_report_battery_time(-REPORT_BATTERY_INTERVAL),
    m_report_status_battery(false),
    m_simulation_mode(false),
    m_emg_beep(false){
}

void GcData::init() {
  //  set pin modes

  if(USE_EMG) {
    pinMode(EMG_SENSOR_PIN, INPUT);
  }
  pinMode(BUZZER_PIN, OUTPUT);

  // initialize various sensors

  if(USE_IMU_1_BNO055) {

    if(!m_bno_1.begin())
    {
      DEBUG_LOG("could not find BNO055 1 on I2C bus");
      error_tone();
    } else {
      DEBUG_LOG("BNO055 1 setup OK");
      m_bno_1.setExtCrystalUse(false);
    }
  }

  if (USE_IMU_2_MMA8452) {
    m_mma_2.init(SCALE_2G, ODR_400);
  }

  // setup battery gauge
  lipo.begin();
  lipo.quickStart();

  read_battery_charge();
}

void GcData::read_battery_charge() {
    float battery_charge = lipo.getSOC();
    battery_charge = min(battery_charge, 100.0);
    p_battery_charge = battery_charge;
    m_gc_client.battery_charge(battery_charge);
}

float GcData::get_gyro_max() {
  if(! USE_IMU_1_BNO055) {
    return 0.0;
  }
  // retrieve the highest gyro rate on 3 axes
  imu::Vector<3> gyro = m_bno_1.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  return max(max(gyro.x(), gyro.y()), gyro.z());
}

void GcData::get_accel_1(float *accel_values) {
  if(! USE_IMU_1_BNO055) {
    accel_values[0] = 0.0;
    accel_values[1] = 0.0;
    accel_values[2] = 0.0;
  } else {
    imu::Vector<3> accel = m_bno_1.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    accel_values[0] = accel.x();
    accel_values[1] = accel.y();
    accel_values[2] = accel.z();
  }
}

void GcData::get_accel_2(float *accel_values) {
  if(! USE_IMU_2_MMA8452) {
    accel_values[0] = 0.0;
    accel_values[1] = 0.0;
    accel_values[2] = 0.0;
  } else {
    m_mma_2.read();
    accel_values[0] = m_mma_2.cx * 10.0;
    accel_values[1] = m_mma_2.cy * 10.0;
    accel_values[2] = m_mma_2.cz * 10.0;
  }
}


uint16_t GcData::read_emg() {
  if(! USE_EMG) {
    // emg disabled
    return 0;
  }

  if(m_simulation_mode) {
    unsigned long milliseconds = millis();
    unsigned long seconds = milliseconds / 1000;
    uint16_t minRand;
    uint16_t maxRand;
    if(seconds % 60 == 0 || seconds % 61 == 0 || seconds % 62 == 0 || seconds % 63 == 0
       || seconds % 64 == 0 || seconds % 65 == 0
    ) {
      // every N seconds
      minRand = 350;
      maxRand = 600;
    }  else {
      minRand = 85;
      maxRand = 125;
    }
    return rand() % (maxRand-minRand+1) + minRand;
  } else {
    return analogRead(A0);
  }
}

void GcData::emg_beep(uint16_t emg_value) {
  if(! m_emg_beep) {
    return;
  }

  if(emg_value > 1000) {
    tone(BUZZER_PIN, 2000, 80);
  } else if(emg_value > 500) {
    tone(BUZZER_PIN, 1200, 80);
  } else if(emg_value > 300) {
    tone(BUZZER_PIN, 800, 80);
  }

}

void GcData::report_battery_charge() {
  DEBUG_LOG("Reporting battery charge");
  read_battery_charge();
  m_last_report_battery_time = millis();
  m_gc_client.report_battery_charge();
}

bool GcData::need_report_battery_charge() {
  if(m_report_status_battery) {
    // status / battery report request
    m_report_status_battery = false;
    return true;
  }
  if (millis() - m_last_report_battery_time > REPORT_BATTERY_INTERVAL) {
    return true;
  }
  return false;
}

void GcData::queue_status_battery_charge() {
  m_report_status_battery = true;
}

void GcData::collect_data(bool upload_requested) {

  uint16_t emg_value = read_emg();
  emg_beep(emg_value);
  float gyro_max = get_gyro_max();
  float accel1_values[3];
  float accel2_values[3];

  get_accel_1(accel1_values);
  get_accel_2(accel2_values);

  bool button1_state = false;
  bool button2_state = ! digitalRead(BUTTON2_PIN);

  m_gc_client.add_datapoint(emg_value, gyro_max, accel1_values, accel2_values, button1_state, button2_state);

  if(need_report_battery_charge()){
    report_battery_charge();
  }

  if(m_gc_client.need_upload() || upload_requested){
    DEBUG_LOG("need to upload batch");

    // turn on WiFi
    if(MANAGE_WIFI) {
      DEBUG_LOG("enabling wifi");
      WiFi.on();
    }
    DEBUG_LOG("wait for wifi to be available");
    // wait for wifi to be available
    waitFor(WiFi.ready, WIFI_MAX_WAIT);
    DEBUG_LOG("wifi available");

    m_gc_client.upload_batch();

    // turn off wifi
    if(MANAGE_WIFI) {
        DEBUG_LOG("disabling wifi");
        WiFi.off();
    }

  }
}
