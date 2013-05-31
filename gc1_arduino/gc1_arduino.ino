
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

// we will keep updating this to keep track of the lowest value seen
int lowestValueSeen = 1023;

const int numReadings = 60;
int lastReadings[numReadings];
int numReadingsSoFar = 0;

const int segment1_duration = 2; // 2*250ms
const int segment2_duration = 12; // 6*250ms
const int segment3_duration = 8; 
const int segment4_duration = 8; 
const int segment_1_2_gap = 3;
const int segment_2_3_gap = 8; // 2 seconds

// segment 1 boundaries
const int segment1_index_start = 0;
const int segment1_index_end = segment1_index_start + segment1_duration;
// segment 2 boundaries
const int segment2_index_start = segment1_index_end + segment_1_2_gap;
const int segment2_index_end = segment2_index_start + segment2_duration;
// segment 3 boundaries 
const int segment3_index_start = segment2_index_end + segment_2_3_gap;
const int segment3_index_end = segment3_index_start + segment3_duration;
// segment 4 boundaries 
const int segment4_index_start = segment3_index_end;
const int segment4_index_end = segment4_index_start + segment4_duration;



// data buffer
struct EntryStruct {
  unsigned long milliseconds;
  int sensor_value;
  boolean segment1_above;
  boolean segment2_above;
  boolean segment3_above;
  boolean segment4_above;
  byte warning_mode;
};

const int log_records_max = 20;
EntryStruct entries[log_records_max];
int struct_index = 0;

void initialize_readings_array() {
  int i;
  for (i = 0; i < numReadings; i++ ){
    lastReadings[i] = 0;
  }
}

void reset_lowest_value_seen() {
  lowestValueSeen = 1023;
}

int grind_threshold(int lowest_value_seen) {
  return lowest_value_seen + 18;
}

void add_reading(int reading) {
  // shift numbers out of array
  int i;
  for (i = numReadings - 1; i > 0; i-- ) {
    lastReadings[i] = lastReadings[i - 1];
  }
  lastReadings[0] = reading;  
  numReadingsSoFar = numReadingsSoFar + 1;
  
  if (reading < lowestValueSeen) {
    lowestValueSeen = reading;
  }
  
}

boolean segment_above_threshold(int start_index, int end_index) {
  for (int i = start_index; i < end_index; i = i + 1) {
    if( lastReadings[i] >= grind_threshold(lowestValueSeen) ) {
      return true;
    }
  }    
  return false;
}

boolean segment1_is_above() {
  return segment_above_threshold(segment1_index_start, segment1_index_end); // first 1.5 seconds
}

boolean segment2_is_above() {
  return segment_above_threshold(segment2_index_start, segment2_index_end);  // second 1.5 seconds
}

boolean segment3_is_above() {
  return segment_above_threshold(segment3_index_start, segment3_index_end);  // following 3 seconds
}

boolean segment4_is_above() {
    return segment_above_threshold(segment4_index_start, segment4_index_end);  // following 3 seconds  
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
  initialize_readings_array();
  reset_lowest_value_seen();
}

void log_headers() {
  // write headers onto data file
  File dataFile = SD.open(logFileName_c, FILE_WRITE);
  String dataString = "millis,sensorValue,segment1_above,segment2_above,segment3_above,segment4_above,warning_mode";
  dataFile.println(dataString);
  dataFile.close();  
}

void log_data(int sensorValue, 
              boolean segment1_above, 
              boolean segment2_above, 
              boolean segment3_above, 
              boolean segment4_above, 
              byte warning_mode) {
  entries[struct_index] = (EntryStruct){millis(), sensorValue,segment1_above,segment2_above,segment3_above,segment4_above,warning_mode};
  
  if(data_buffer_full()) {
    flush_data_buffer();
  } else {
    struct_index += 1; 
  }
}

String convert_bool(boolean value) {
  if (value) {
    return "1";
  }
  return "0";
}

void flush_data_buffer() {
  File dataFile = SD.open(logFileName_c, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    for( int i = 0; i < log_records_max; i++) {
      EntryStruct entry = entries[i];
      String dataString = String(entry.milliseconds) + "," + \
                          String(entry.sensor_value) + "," + \
                          convert_bool(entry.segment1_above) + "," + \
                          convert_bool(entry.segment2_above) + "," + \
                          convert_bool(entry.segment3_above) + "," + \
                          convert_bool(entry.segment4_above) + "," + \
                          String(entry.warning_mode) + ",";
      dataFile.println(dataString);
      if(useSerial) {
        Serial.println(dataString);
      }      
    }
    
    dataFile.close();
    struct_index = 0;
    
    if( useSerial) {
      Serial.println("wrote to data file (" + String(log_records_max) + " entries)");
      log_free_ram();
    }
  }    
}

boolean data_buffer_full() {
  if (struct_index == log_records_max - 1) {
    return true;
  }
  return false;
}

int sensor_value() {
  sensorValue = analogRead(A0);
  return sensorValue;
}

void loop() {
  
  controlButtonState = digitalRead(controlPin);
  sensorValue = sensor_value();

  // insert current reading into array
  add_reading(sensorValue);
  
  boolean segment1_above = segment1_is_above();
  boolean segment2_above = segment2_is_above();
  boolean segment3_above = segment3_is_above();
  boolean segment4_above = segment4_is_above();
  
  byte warning_mode = 0; // no warning
  if (segment1_above && segment2_above && (segment3_above || segment4_above)) {
    // grind alert
    warning_mode = 2;
  } else if (segment1_above && segment2_above) {
    // grind warning
    warning_mode = 1;
  }
    
  // log to flashcard
  log_data(sensorValue, segment1_above, segment2_above, segment3_above, segment4_above, warning_mode);
  

  if( warning_mode == 1 ) {
    warning_grind();
  }

  if ( warning_mode == 2 ) {
    trigger_grind_alarm();
  }
 
  
  if (controlButtonState == HIGH) {
   tone(soundPin,600);
   delay(50);  
   noTone(soundPin);   
  }
 
  delay(250);
};
