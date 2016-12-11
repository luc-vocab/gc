#ifndef _GC_DATA_STRUCT_H
#define _GC_DATA_STRUCT_H

struct data_point {
  // eventually this will overflow, but the device is only supposed to stay on for one night, so OK
  uint32_t milliseconds;
  uint16_t emg_value;
  int16_t gyro;

  int16_t imu1_accel_x;
  int16_t imu1_accel_y;
  int16_t imu1_accel_z;

  int16_t imu2_accel_x;
  int16_t imu2_accel_y;
  int16_t imu2_accel_z;

  uint8_t flags;
};


#endif /* _GC_DATA_STRUCT_H */
