
#include "common.h"

void validation_tone() {
  tone(BUZZER_PIN, 600, 150);
  delay(150);
  tone(BUZZER_PIN, 900, 150);
}
