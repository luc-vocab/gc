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
    m_emg_beep(false),
    m_tap_to_upload(false){
      memset(&m_last_data_point, 0, sizeof(data_point));
}

void GcData::init() {
  //  set pin modes

  if(USE_EMG) {
    pinMode(EMG_SENSOR_PIN, INPUT);
  }
  pinMode(BUZZER_PIN, OUTPUT);

  // initialize various sensors
  pinMode(FAST_MODE_LED_PIN, OUTPUT);

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

void GcData::get_accel_1(int16_t *accel_values) {
  if(! USE_IMU_1_BNO055) {
    accel_values[0] = 0.0;
    accel_values[1] = 0.0;
    accel_values[2] = 0.0;
  } else {
    /*
    imu::Vector<3> accel = m_bno_1.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    accel_values[0] = accel.x();
    accel_values[1] = accel.y();
    accel_values[2] = accel.z();
    */

    // DEBUG_LOG("accel: x " + String(accel.x()) + " y: " + String(accel.y()) + " z: " + String(accel.z()));

    m_bno_1.getRawVector(Adafruit_BNO055::VECTOR_LINEARACCEL, accel_values);
    //DEBUG_LOG("accel: x " + String(raw_vector[0]) + " y: " + String(raw_vector[1]) + " z: " + String(raw_vector[2]));
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

bool GcData::tap_received() {
  // check whether we've received a tap, indicating upload was requested
  bool result = false;

  if(USE_IMU_2_MMA8452)
  {
    byte tapStat = m_mma_2.readTap();
    if(tapStat > 0)
    {
      if(m_mma_2.isSingleTap(tapStat)) {
        DEBUG_LOG("received single tap");
      }
      if(m_mma_2.isDoubleTap(tapStat)) {
        DEBUG_LOG("received double tap");
        result = true;
      }
    }
  }

  return result;
}

bool GcData::fast_movement(const data_point &dp1, const data_point &dp2)
{
  /*
  if(abs(dp1.imu1_accel_x - dp2.imu1_accel_x) > FAST_MOVEMENT_BNO055_LINEAR_ACCEL )
    return true;
  if(abs(dp1.imu1_accel_y - dp2.imu1_accel_y) > FAST_MOVEMENT_BNO055_LINEAR_ACCEL )
      return true;
  if(abs(dp1.imu1_accel_z - dp2.imu1_accel_z) > FAST_MOVEMENT_BNO055_LINEAR_ACCEL )
      return true;
      */

  if(abs(dp1.imu2_accel_x - dp2.imu2_accel_x) > FAST_MOVEMENT_MMA8452_ACCEL )
    return true;
  if(abs(dp1.imu2_accel_y - dp2.imu2_accel_y) > FAST_MOVEMENT_MMA8452_ACCEL )
      return true;
  if(abs(dp1.imu2_accel_z - dp2.imu2_accel_z) > FAST_MOVEMENT_MMA8452_ACCEL )
      return true;


  return false;
}

void GcData::collect_data(bool upload_requested) {

  data_point dp;
  memset(&dp, 0, sizeof(data_point));

  // eventually this will overflow, but the device is only on for one night,
  // so should be OK
  dp.milliseconds = millis();

  dp.emg_value = read_emg();
  emg_beep(dp.emg_value);

  float gyro_max = get_gyro_max();
  int16_t gyro_value = gyro_max * 100.0;
  dp.gyro = gyro_value;


  int16_t accel1_values[3];
  float accel2_values[3];

  get_accel_1(accel1_values);
  get_accel_2(accel2_values);

  dp.imu1_accel_x = accel1_values[0];
  dp.imu1_accel_y = accel1_values[1];
  dp.imu1_accel_z = accel1_values[2];

  dp.imu2_accel_x = accel2_values[0] * 1000.0;
  dp.imu2_accel_y = accel2_values[1] * 1000.0;
  dp.imu2_accel_z = accel2_values[2] * 1000.0;

  m_gc_client.add_datapoint(dp);

  if(fast_movement(dp, m_last_data_point))
  {
    digitalWrite(FAST_MODE_LED_PIN, HIGH);
  } else {
    digitalWrite(FAST_MODE_LED_PIN, LOW);
  }

  m_last_data_point = dp;

  if(need_report_battery_charge()){
    report_battery_charge();
  }

  bool tapReceived = tap_received();
  if (tapReceived && m_tap_to_upload) {
    validation_tone();
    upload_requested = true;
  }

  if(m_gc_client.need_upload() || upload_requested){
    DEBUG_LOG("need to upload batch");
    m_gc_client.upload_batch();
  }
}

void GcData::enable_tap_to_upload(bool enable)
{
  m_tap_to_upload = enable;
}
