#include "application.h"
#include "SparkFunMAX17043.h" // Include the SparkFun MAX17043 library
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "imumaths.h"


/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055();

#define BUZZER_PIN A4
#define BUTTON1_PIN D2
#define BUTTON2_PIN D3

bool s_report_button1_state = false;
bool s_report_button2_state = false;

bool s_start_battery = false;
bool s_report_battery_state = false;

bool s_setup_bno055 = false;
bool s_report_imu_state = false;

int start_batt(String input);
int stop_batt(String input);
int start_bno055(String input);
int stop_bno055(String input);
int test_spkr(String input);
void button1();
void button2();
void report_battery_state();
void report_imu_state();

Timer battery_timer(5000, report_battery_state);
Timer imu_timer(2000, report_imu_state);

void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  attachInterrupt(BUTTON1_PIN, button1, CHANGE);
  attachInterrupt(BUTTON2_PIN, button2, CHANGE);

  // functions
  Particle.function("start_batt", start_batt);
  Particle.function("stop_batt", stop_batt);
  Particle.function("test_spkr", test_spkr);
  Particle.function("start_bno055", start_bno055);
  Particle.function("stop_bno055", stop_bno055);

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

int start_bno055(String input)
{
  s_setup_bno055  = true;
  return 0;
}

int stop_bno055(String input)
{
  imu_timer.stop();
  return 0;
}

void report_imu_state()
{
  s_report_imu_state = true;
}

void setup_bno055()
{

  Particle.publish("imu", "Starting BNO055 setup");
  /* Initialise the sensor */
  if(!bno.begin())
  {
    Particle.publish("imu", "could not find BNO055 on I2C bus");

  } else {

    delay(1000);

    Particle.publish("imu", "BNO055 setup OK");

    bno.setExtCrystalUse(false);

    // if setup is successful, start Timer
    imu_timer.start();

  }
  s_setup_bno055 = false;
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

  if(s_setup_bno055)
  {
      setup_bno055();
  }

  if(s_report_imu_state)
  {

    uint8_t system, gyro, accel, mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    String status = String::format("Orientation: x=%.1f y=%.1f z=%.1f Calib s:%d g:%d a:%d m:%d",
      euler.x(),
      euler.y(),
      euler.z(),
      system,
      gyro,
      accel,
      mag
    );

    /*
    Particle.publish("imu","CALIBRATION: Sys="+String(system)+" gyro="+String(gyro)+" accel="+String(accel)+" mag="+String(mag) );
    Particle.publish("imu", "Orientation: x="+String(euler.x())+" y="+String(euler.y()) + " z=" +String(euler.z()));
    */
    Particle.publish("imu", status);

    s_report_imu_state = false;
  }

  delay(250);
}
