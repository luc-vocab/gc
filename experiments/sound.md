playing sound using arduino:

const int soundPin = 5;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  tone(soundPin,600);
  delay(200);
  noTone(soundPin);
  delay(100);
}


using shure se215 headphones:

the headphones are around the 100k resistor
100k/10k: quite loud
100k/27k: medium loud
100k/50k: medium
100k/82k: low volume
100k/270k: faint volume


