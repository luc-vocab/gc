
#include "common.h"

void validation_tone() {
  tone(BUZZER_PIN, 600, 150);
  delay(150);
  tone(BUZZER_PIN, 900, 150);
}


void error_tone() {
  tone(BUZZER_PIN, 300, 150);
  delay(500);
  tone(BUZZER_PIN, 300, 150);
  delay(500);
  tone(BUZZER_PIN, 300, 150);
}
