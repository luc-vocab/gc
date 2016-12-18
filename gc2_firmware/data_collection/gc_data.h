#ifndef _GC_DATA_H_
#define _GC_DATA_H_

#include "gc_client.h"
#include "common.h"
#include "data_struct.h"

#include "SparkFunMAX17043.h" // Include the SparkFun MAX17043 library, battery gauge
#include "math.h"

// required includes for BNO055
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "imumaths.h"

// for MMA8452
#include "SFE_MMA8452Q.h"

#define REPORT_BATTERY_INTERVAL 120000 // every 2mn

#define STD_DEV_SAMPLES 20

#define FAST_MOVEMENT_BNO055_LINEAR_ACCEL 30
#define FAST_MOVEMENT_MMA8452_ACCEL 300

#define FAST_MOVEMENT_DURATION 4000
#define SLOW_MOVEMENT_INTERVAL 2000

class GcData {
public:
  GcData(GcClient &gc_client);
  // initialize various sensors (should be called during setup())
  void init();
  void collect_data(bool upload_requested);
  // report status and battery charge at next cycle
  void queue_status_battery_charge();
  void set_simulation_mode(bool simulation_mode) { m_simulation_mode = simulation_mode; }
  void set_emg_beep(bool emg_beep) { m_emg_beep = emg_beep; }
  void toggle_emg_beep() { if (!m_emg_beep) { m_emg_beep = true; } else { m_emg_beep = false;} }

  void enable_tap_to_upload(bool enable);

  int p_battery_charge;

private:
  void report_battery_charge();
  void read_battery_charge();
  bool need_report_battery_charge();
  void get_gyro_1(data_point *dp);
  void get_accel_1(int16_t *accel_values);
  void get_accel_2(float *accel_values);
  bool tap_received();
  bool fast_movement(const data_point &dp1, const data_point &dp2);
  void process_stddev(const data_point &dp);

  bool m_simulation_mode;
  bool m_emg_beep;
  bool m_report_status_battery;
  bool m_tap_to_upload;
  uint32_t m_last_report_battery_time;
  GcClient &m_gc_client;

  #if USE_IMU_1_BNO055
  Adafruit_BNO055 m_bno_1;
  #endif

  #if USE_IMU_2_MMA8452
  MMA8452Q m_mma_2;
  #endif

  data_point m_last_data_point;
  uint32_t m_fast_movement_start_millis;
  uint32_t m_last_datapoint_time;

  char m_num_data_points;
  data_point m_data_points[STD_DEV_SAMPLES];
};


#endif /* _GC_DATA_H_ */
