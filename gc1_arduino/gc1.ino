
#include <SD.h>

const boolean useSerial = false;

const int soundPin = 5;
const int controlPin = 7;

int controlButtonState = 0;
int sensorValue = 0;

// *** SD card variables ***
Sd2Card card;
SdVolume volume;
SdFile root;
String logFileName = "gcLog_";

char logFileName_c[40];
// sd card pin
const int chipSelect = 3;    

// int numGrindsInARow = 0;

const int numReadings = 10;
int lastReadings[numReadings];

void initialize_readings_array() {
  int i;
  for (i = 0; i < numReadings; i++ ){
    lastReadings[i] = 0;
  }
}

int add_reading_and_avg(int reading) {
  // shift numbers out of array
  int i;
  for (i = numReadings - 1; i > 0; i-- ) {
    lastReadings[i] = lastReadings[i - 1];
  }
  lastReadings[0] = reading;  
  
  // now perform average
  int sumReadings = 0;
  for (i = 0; i < numReadings; i = i + 1) {
    sumReadings += lastReadings[i];
  }    
  
  return (sumReadings / numReadings);
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  if (useSerial) {
    Serial.begin(9600);
  };
 
 initialize_readings_array();
 
  // ******* sd card init ********
  pinMode(10, OUTPUT);
  
  if (!SD.begin(chipSelect)) {
    error_beep();
    return;
  }
  
  File root = SD.open("/");
 
  if ( ! root ) {
    error_beep();
  }
  
  root.rewindDirectory();
  int numFiles = 1;
  
   while(true) {
     File entry = root.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     numFiles++; 
   }

   logFileName = logFileName + String(numFiles) + ".txt";
   root.close();
   
   logFileName.toCharArray(logFileName_c, logFileName.length() + 1);
   
   if( useSerial) {
     Serial.print("datalog file name: [");
     Serial.print(logFileName_c);
     Serial.println("]");
   }
   
  // set analog reference
  //analogReference(EXTERNAL);

   start_program_beep(); 
}

void valid_beep() {
  tone(soundPin,200);
  delay(200);
  tone(soundPin,400);
  delay(200);
  noTone(soundPin);  
  delay(500);
}

void error_beep() {
  tone(soundPin,600);
  delay(100);
  noTone(soundPin);
  delay(50);
  tone(soundPin,600);
  delay(100);
  noTone(soundPin);
  delay(50);
  tone(soundPin,600);
  delay(100);
  noTone(soundPin);
}

void start_program_beep() {
  if (useSerial) {
    Serial.println("start program");
  };
  tone(soundPin,200);
  delay(400);
  noTone(soundPin);

  tone(soundPin,200);
  delay(200);
  tone(soundPin,400);
  delay(200);
  tone(soundPin,600);
  delay(200);  
  noTone(soundPin);   
  
}

void alert_grind() {
  tone(soundPin,600);
  delay(5000);  
  noTone(soundPin);   
  // numGrindsInARow = 0;
  
}


void alert_reset_step() {
  tone(soundPin,200);
  delay(100);
  tone(soundPin,400);
  delay(100);
  noTone(soundPin);   
  delay(200);
}

void alert_reset_loop_iteration() {
  tone(soundPin,600);
  delay(500);  
  noTone(soundPin);   
}

void wait_for_reset(){
  // wait for user to reset

  boolean sequenceCompleted = false;
  boolean highSelected = false;

  while ( !sequenceCompleted) {
      controlButtonState = digitalRead(controlPin);
      
      if (controlButtonState == HIGH && ! highSelected) {
        highSelected = true;
        alert_reset_step();
      }
      
      if (highSelected == true && controlButtonState == LOW) {
        sequenceCompleted = true;
        alert_reset_step();
      }
      
      alert_reset_loop_iteration();
      
      delay(100);
    
  }

  alert_reset_step();
  
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


void loop() {
  
  // wait_for_reset();
  
  controlButtonState = digitalRead(controlPin);
  sensorValue = analogRead(A0);

  int average = add_reading_and_avg(sensorValue);
  
  if (useSerial) {
    Serial.println("loop");
    Serial.print("sensorValue: ");
    Serial.print(sensorValue);
    Serial.print(" average: ");
    Serial.println(average);
    if (controlButtonState == HIGH) {
      Serial.println("state: high");
    } else {
      Serial.println("state: low");      
    }
    
    Serial.print("free ram: ");
    Serial.println(freeRam());
  }
  

  if (average > 56) {
    alert_grind();
    wait_for_reset();
    initialize_readings_array();
  }
  
  /*
  if (sensorValue > 100 ) {
    // grind value
    numGrindsInARow += 1;
  } else {
    numGrindsInARow = 0;
  }
  
  if (numGrindsInARow >= 5 ) {
    alert_grind();
  }
  */
  
 if (controlButtonState == HIGH) {
  tone(soundPin,600);
  delay(50);  
  noTone(soundPin);   
 }
  
  if (useSerial) {
    Serial.print("attempting to open file: ");
    Serial.println(logFileName_c);
  }
  
  File dataFile = SD.open(logFileName_c, FILE_WRITE);

  // if the file is available, write to it:
  String dataString = String(millis()) + "," + String(sensorValue) + "," + String(average) + ",";
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    
    if( useSerial) {
      Serial.println("wrote to data file");
      //Serial.println(dataString);
    }
  }  
  
  delay(1000);
};
