#ifndef _GC_DATA_STRUCT_H
#define _GC_DATA_STRUCT_H


struct data_point {
  // eventually this will overflow, but the device is only supposed to stay on for one night, so OK
  uint32_t milliseconds;
  
  int16_t gyro1_x;
  int16_t gyro1_y;
  int16_t gyro1_z;

  int16_t imu1_accel_x;
  int16_t imu1_accel_y;
  int16_t imu1_accel_z;

  int16_t imu2_accel_x;
  int16_t imu2_accel_y;
  int16_t imu2_accel_z;

  uint8_t flags1;
  uint8_t flags2;
  uint8_t flags3;
  uint8_t flags4;
};


#endif /* _GC_DATA_STRUCT_H */
