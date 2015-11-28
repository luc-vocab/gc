#ifndef _GC_DATA_H_
#define _GC_DATA_H_

#include "gc_client.h"
#include "SparkFunMAX17043.h" // Include the SparkFun MAX17043 library, battery gauge
#include "SparkFunLSM9DS1.h" // include Sparkfun LSM9DS1 library, IMU
#include "math.h"

#define EMG_SENSOR_PIN A0
#define BUZZER_PIN A4

#define LSM9DS1_M	0x1E
#define LSM9DS1_AG	0x6B


class GcData {
public:
  GcData();
  // initialize various sensors (should be called during setup())
  void init(GcClient *gc_client);
  void collect_data();

private:
  void report_battery_charge();
  float get_gyro_max();
  void get_accel(float *accel_values);

  GcClient *m_gc_client;
  LSM9DS1 m_imu;
};


#endif /* _GC_DATA_H_ */
