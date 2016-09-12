
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

// file handle for logging
File logDataFile;

// sd card pin
const int chipSelect = 3;    

// we will keep updating this to keep track of the lowest value seen
int lowestValueSeen = 1023;

// considered a grind if value is >30 + lowestValueSeen
const int grindThreshold = 30;

const byte LEVEL_NO_GRIND      = 0;
const byte LEVEL_GRIND_NO_WARN = 1;
const byte LEVEL_GRIND_WARN_1  = 2;
const byte LEVEL_GRIND_WARN_2  = 3;
const byte LEVEL_GRIND_ALERT   = 4;

// no warning for first 5 grinds
const int warn0NumGrinds = 10;
// allow 5 grinds at the first warning level
const int warn1NumGrinds = 5;
// allow 5 grinds at hte second warning level
const int warn2NumGrinds = 5;

const int flush_every = 20;
const int log_records_max = 50;

// warning_mode array
byte warning_mode_array[log_records_max];

unsigned long numEntriesSoFar = 0;

void reset_lowest_value_seen() {
  lowestValueSeen = 1023;
}

int grind_threshold(int lowest_value_seen) {
  return lowest_value_seen + 30;
}

void add_reading(int reading, byte warning_mode) {
  // shift numbers out of array
  int i;
  for (i = log_records_max - 1; i > 0; i-- ) {
    warning_mode_array[i] = warning_mode_array[i - 1];
  }
  warning_mode_array[0] = warning_mode;
  
  numEntriesSoFar = numEntriesSoFar + 1;
  
  // memorize lowest value seen if necessary
  if (reading < lowestValueSeen) {
    lowestValueSeen = reading;
  }

  // flushing will be automatic
  log_data_to_sd(millis(), reading, warning_mode);
  
}

// number of grinds in the 10-second window so far
int num_grinds() {
  
  int result = 0;
  for(int i = 0; i < log_records_max; i++) {
    if (warning_mode_array[i] == LEVEL_GRIND_ALERT) {
      // stop counting at that point, a grind alert already happened
      break;
    }
    if (warning_mode_array[i] > LEVEL_NO_GRIND ) {
      result += 1;
    }
  }
  
  return result;
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  if (useSerial) {
    Serial.begin(9600);
   
  };
 
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
       break;
     }
     entry.close();
     numFiles++; 
  
    log_free_ram();
     
   }

   logFileName = logFileName + String(numFiles) + ".txt";
   root.close();
   
   logFileName.toCharArray(logFileName_c, logFileName.length() + 1);
   
   if( useSerial) {
     Serial.print("datalog file name: [");
     Serial.print(logFileName_c);
     Serial.println("]");
   }
   
   // initialize warning_mode_array
   for(int i = 0; i < log_records_max; i++) {
     warning_mode_array[i] = LEVEL_NO_GRIND;
   }
   
   logDataFile = SD.open(logFileName_c, FILE_WRITE);
   
   log_headers();
   
   start_program_beep(); 

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

void warning_grind() {
  tone(soundPin,400);
  delay(50);
  noTone(soundPin);
}

void warning_grind_high() {
  tone(soundPin,800);
  delay(50);
  noTone(soundPin);
}

void alert_grind() {
  tone(soundPin,600);
  delay(5000);  
  noTone(soundPin);   
}


void alert_reset_step() {
  tone(soundPin,200);
  delay(50);
  tone(soundPin,400);
  delay(50);
  noTone(soundPin);   
  delay(50);
}

void alert_reset_loop_iteration() {
  tone(soundPin,600);
  delay(100);  
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

void log_free_ram() {
  if (useSerial) {
    Serial.print("free ram: ");
    Serial.println(freeRam()); 
  }
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void trigger_grind_alarm(){
  alert_grind();
  wait_for_reset();
  reset_lowest_value_seen();
}

void log_headers() {
  // write headers onto data file
  String dataString = "millis,sensorValue,warning_mode,";
  logDataFile.println(dataString);
}

void log_data_to_sd(unsigned long milliseconds, int sensor_value, byte warning_mode) {
  String dataString = String(milliseconds) + "," + \
                          String(sensor_value) + "," + \
                          String(warning_mode) + ",";
    logDataFile.println(dataString);

  if (numEntriesSoFar % flush_every == 0) {
    logDataFile.flush();
    
    if(useSerial) {
      // Serial.println(dataString);
      Serial.println("flushed");
      log_free_ram();
    }    
  }
}


int sensor_value() {
  sensorValue = analogRead(A0);
  return sensorValue;
}

void loop() {
  
  controlButtonState = digitalRead(controlPin);
  sensorValue = sensor_value();

  byte warning_mode = LEVEL_NO_GRIND; // no warning

  boolean grindDetected = false;
  if( sensorValue >= grind_threshold(lowestValueSeen) ) {
    grindDetected = true;
  }

  // how many grinds do we have so far ?
  int num_grinds_in_window = num_grinds();
  
  if( grindDetected ) {
    warning_mode = LEVEL_GRIND_NO_WARN;
  }
  if( grindDetected && num_grinds_in_window > warn0NumGrinds ) {
    warning_mode = LEVEL_GRIND_WARN_1;
  }
  if ( grindDetected && num_grinds_in_window > warn0NumGrinds + warn1NumGrinds ) {
    warning_mode = LEVEL_GRIND_WARN_2;    
  }
  if ( grindDetected && num_grinds_in_window > warn0NumGrinds + warn1NumGrinds + warn2NumGrinds ) {
    warning_mode = LEVEL_GRIND_ALERT;
  }  
    
  // insert current reading into array
  add_reading(sensorValue, warning_mode);
  
  if( warning_mode == LEVEL_GRIND_WARN_1 ) {
    warning_grind();
  }

  if( warning_mode == LEVEL_GRIND_WARN_2 ) {
    warning_grind_high();
  }
  
  if ( warning_mode == LEVEL_GRIND_ALERT ) {
    trigger_grind_alarm();
  }
 
  
  if (controlButtonState == HIGH) {
   tone(soundPin,600);
   delay(50);  
   noTone(soundPin);   
  }
 
  delay(250);
};
