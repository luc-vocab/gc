#include "gc_data.h"
#include "common.h"

GcData::GcData(GcClient &gc_client) : m_gc_client(gc_client) {

}

void GcData::init() {
  //  set pin modes
  pinMode(EMG_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // initialize various sensors

  // IMU setup
  m_imu.settings.device.commInterface = IMU_MODE_I2C;
  m_imu.settings.device.mAddress = LSM9DS1_M;
  m_imu.settings.device.agAddress = LSM9DS1_AG;

  if (!m_imu.begin())
  {
    DEBUG_LOG("Failed to communicate with LSM9DS1.");
  }

  // setup battery gauge
  lipo.begin();
  lipo.quickStart();


}

void GcData::report_battery_charge() {
    m_gc_client.battery_charge(lipo.getSOC());
}

float GcData::get_gyro_max() {
  // retrieve the highest gyro rate on 3 axes
  m_imu.readGyro();
  float gyro_x = m_imu.calcGyro(m_imu.gx);
  float gyro_y = m_imu.calcGyro(m_imu.gy);
  float gyro_z = m_imu.calcGyro(m_imu.gz);

  return max(max(gyro_x, gyro_y), gyro_z);
}

void GcData::get_accel(float *accel_values) {
  m_imu.readAccel();
  accel_values[0] = m_imu.calcAccel(m_imu.ax);
  accel_values[1] = m_imu.calcAccel(m_imu.ay);
  accel_values[2] = m_imu.calcAccel(m_imu.az);
}

void GcData::collect_data() {
  uint16_t emg_value = analogRead(A0);
  float gyro_max = get_gyro_max();
  float accel_values[3];
  get_accel(accel_values);

  float accel_x = accel_values[0];
  float accel_y = accel_values[1];
  float accel_z = accel_values[2];

  m_gc_client.add_datapoint(emg_value, gyro_max, accel_x, accel_y, accel_z);

  if(m_gc_client.need_upload()){
    DEBUG_LOG("need to upload batch");
    // get battery data
    report_battery_charge();

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
