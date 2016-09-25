#include "application.h"
#include "SparkFunMAX17043.h" // Include the SparkFun MAX17043 library

#define BUZZER_PIN A4
#define BUTTON1_PIN D2
#define BUTTON2_PIN D3

bool s_battery_setup = false;
bool s_stream_battery = false;

int start_batt(String input);
int stop_batt(String input);
int test_spkr(String input);


void setup() {
  Particle.function("start_batt", start_batt);
  Particle.function("stop_batt", stop_batt);
  Particle.function("test_spkr", test_spkr);
}

int start_batt(String input)
{
  return 0;
}

int stop_batt(String input)
{
  return 0;
}

int test_spkr(String input)
{
  tone(BUZZER_PIN, 600, 150);
  delay(150);
  tone(BUZZER_PIN, 900, 150);

  return 0;
};

void loop() {


  delay(2000);
}
