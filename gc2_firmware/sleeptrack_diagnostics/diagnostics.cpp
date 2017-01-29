#include "application.h"
#include "SparkFunMAX17043.h" // Include the SparkFun MAX17043 library
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "imumaths.h"
#include "Adafruit_DRV2605.h"

PRODUCT_ID(1523);
PRODUCT_VERSION(2);

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno1 = Adafruit_BNO055(-1, BNO055_ADDRESS_A);
Adafruit_BNO055 bno2 = Adafruit_BNO055(-1, BNO055_ADDRESS_B);

Adafruit_DRV2605 drv;

#define BUZZER_PIN A4
#define BUTTON1_PIN D2
#define BUTTON2_PIN D3

bool s_report_button1_state = false;
bool s_report_button2_state = false;

bool s_start_battery = false;
bool s_report_battery_state = false;

bool s_setup_bno055_1 = false;
bool s_report_imu_state_1 = false;
bool s_setup_bno055_2 = false;
bool s_report_imu_state_2 = false;


int start_batt(String input);
int stop_batt(String input);
int start_bno_1(String input);
int stop_bno_1(String input);
int start_bno_2(String input);
int stop_bno_2(String input);
int test_spkr(String input);
void button1();
void button2();
void report_battery_state();
void report_imu_state_1();
void report_imu_state_2();
int test_motor(String input);

Timer battery_timer(5000, report_battery_state);
Timer imu_timer_1(2000, report_imu_state_1);
Timer imu_timer_2(2000, report_imu_state_2);

void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  attachInterrupt(BUTTON1_PIN, button1, CHANGE);
  attachInterrupt(BUTTON2_PIN, button2, CHANGE);

  // functions
  Particle.function("start_batt", start_batt);
  Particle.function("stop_batt", stop_batt);
  Particle.function("test_spkr", test_spkr);
  Particle.function("start_bno_1", start_bno_1);
  Particle.function("stop_bno_1", stop_bno_1);
  Particle.function("start_bno_2", start_bno_2);
  Particle.function("stop_bno_2", stop_bno_2);
  Particle.function("test_motor", test_motor);

}

void button1()
{
  s_report_button1_state = true;
}

void button2()
{
  s_report_button2_state = true;
}

int start_batt(String input)
{
  s_start_battery = true;
  return 0;
}

int stop_batt(String input)
{
  battery_timer.stop();
  return 0;
}

int test_spkr(String input)
{
  tone(BUZZER_PIN, 600, 150);
  delay(150);
  tone(BUZZER_PIN, 900, 150);

  return 0;
};

void setup_battery()
{
  lipo.begin(); // Initialize the MAX17043 LiPo fuel gauge
	lipo.quickStart();

  Particle.publish("battery", "battery setup complete");

  battery_timer.start();
}

void report_battery_state()
{
  s_report_battery_state = true;
}

// BNO1 setup
// ===========================

int start_bno_1(String input)
{
  s_setup_bno055_1  = true;
  return 0;
}

int stop_bno_1(String input)
{
  imu_timer_1.stop();
  return 0;
}

void report_imu_state_1()
{
  s_report_imu_state_1 = true;
}

void setup_bno055_1()
{

  Particle.publish("imu1", "Starting BNO055 setup");
  /* Initialise the sensor */
  if(!bno1.begin())
  {
    Particle.publish("imu1", "could not find BNO055 on I2C bus");

  } else {

    delay(1000);

    Particle.publish("imu1", "BNO055 setup OK");

    bno1.setExtCrystalUse(false);

    // if setup is successful, start Timer
    imu_timer_1.start();

  }
  s_setup_bno055_1 = false;
}

// BNO2 setup
// ===========================

int start_bno_2(String input)
{
  s_setup_bno055_2  = true;
  return 0;
}

int stop_bno_2(String input)
{
  imu_timer_2.stop();
  return 0;
}

void report_imu_state_2()
{
  s_report_imu_state_2 = true;
}

void setup_bno055_2()
{

  Particle.publish("imu2", "Starting BNO055 setup");
  /* Initialise the sensor */
  if(!bno2.begin())
  {
    Particle.publish("imu2", "could not find BNO055 on I2C bus");

  } else {

    delay(1000);

    Particle.publish("imu2", "BNO055 setup OK");

    bno2.setExtCrystalUse(false);

    // if setup is successful, start Timer
    imu_timer_2.start();

  }
  s_setup_bno055_2 = false;
}

int test_motor(String input)
{
  // setup DRV2605
  drv.begin();
  drv.selectLibrary(1);
  drv.setMode(DRV2605_MODE_INTTRIG);

  drv.setWaveform(0, 47);  // play effect
  drv.setWaveform(1, 0);       // end waveform

  // play the effect!
  drv.go();

  return 0;
}


void loop() {
  if (s_report_button1_state)
  {
    bool button1_state = digitalRead(BUTTON1_PIN);
    Particle.publish("button1", String(button1_state));
    s_report_button1_state = false;
  }

  if (s_report_button2_state)
  {
    bool button2_state = digitalRead(BUTTON2_PIN);
    Particle.publish("button2", String(button2_state));
    s_report_button2_state = false;
  }

  if(s_start_battery)
  {
    setup_battery();
    s_start_battery = false;
  }

  if (s_report_battery_state)
  {
    double voltage = lipo.getVoltage();
  	double soc = lipo.getSOC();

    Particle.publish("battery", "volt: " + String(voltage) + " pct: " + String(soc));
    s_report_battery_state = false;
  }

  // bno1
  // =====
  if(s_setup_bno055_1)
  {
      setup_bno055_1();
  }

  if(s_report_imu_state_1)
  {

    uint8_t system, gyro, accel, mag = 0;
    bno1.getCalibration(&system, &gyro, &accel, &mag);
    imu::Vector<3> euler = bno1.getVector(Adafruit_BNO055::VECTOR_EULER);

    String status = String::format("Orient: x=%.0f y=%.0f z=%.0f Cal s:%d g:%d a:%d m:%d",
      euler.x(),
      euler.y(),
      euler.z(),
      system,
      gyro,
      accel,
      mag
    );

    Particle.publish("imu1", status);

    s_report_imu_state_1 = false;
  }

  // bno2
  // =====
  if(s_setup_bno055_2)
  {
      setup_bno055_2();
  }

  if(s_report_imu_state_2)
  {

    uint8_t system, gyro, accel, mag = 0;
    bno2.getCalibration(&system, &gyro, &accel, &mag);
    imu::Vector<3> euler = bno2.getVector(Adafruit_BNO055::VECTOR_EULER);

    String status = String::format("Orient: x=%.0f y=%.0f z=%.0f Cal s:%d g:%d a:%d m:%d",
      euler.x(),
      euler.y(),
      euler.z(),
      system,
      gyro,
      accel,
      mag
    );

    Particle.publish("imu2", status);

    s_report_imu_state_2 = false;
  }


  delay(250);
}
