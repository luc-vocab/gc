#include "application.h"
#include "SparkFunMAX17043.h" // Include the SparkFun MAX17043 library

#define BUZZER_PIN A4
#define BUTTON1_PIN D2
#define BUTTON2_PIN D3

bool s_report_button1_state = false;
bool s_report_button2_state = false;

bool s_start_battery = false;
bool s_report_battery_state = false;

int start_batt(String input);
int stop_batt(String input);
int test_spkr(String input);
void button1();
void button2();
void report_battery_state();

Timer battery_timer(5000, report_battery_state);

void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  attachInterrupt(BUTTON1_PIN, button1, CHANGE);
  attachInterrupt(BUTTON2_PIN, button2, CHANGE);

  // functions
  Particle.function("start_batt", start_batt);
  Particle.function("stop_batt", stop_batt);
  Particle.function("test_spkr", test_spkr);


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

  delay(250);
}
