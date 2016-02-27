#ifndef _GC_DATA_H_
#define _GC_DATA_H_

#include "gc_client.h"
#include "SparkFunMAX17043.h" // Include the SparkFun MAX17043 library, battery gauge
#include "SparkFunLSM9DS1.h" // include Sparkfun LSM9DS1 library, IMU
#include "math.h"

#define LSM9DS1_M	0x1E
#define LSM9DS1_AG	0x6B

#define SIMULATION_MODE true
#define REPORT_BATTERY_INTERVAL 120000 // every 2mn


class GcData {
public:
  GcData(GcClient &gc_client);
  // initialize various sensors (should be called during setup())
  void init();
  void collect_data(bool upload_requested);
  void report_battery_charge();

  int p_battery_charge;

private:
  void read_battery_charge();
  bool need_report_battery_charge();
  float get_gyro_max();
  void get_accel(float *accel_values);
  uint16_t read_emg();

  uint32_t m_last_report_battery_time;
  GcClient &m_gc_client;
  LSM9DS1 m_imu;
};


#endif /* _GC_DATA_H_ */
