const int numReadings = 1000;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;
int micVal  = 0;
int micPin  = A6;
int vibPin = 7;
int vibVal = 0;
#include <SD.h>
#include <SPI.h>
#include <AudioZero.h>

void setup() {
  Serial.begin(9600);
  pinMode(micPin, INPUT);
  pinMode(vibPin, INPUT);
  pinMode(12, OUTPUT);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(SDCARD_SS_PIN)) {
    Serial.println(" failed!");
    while (true);
  }
  Serial.println(" done.");

  // 44100kHz stereo => 88200 sample rate
    AudioZero.begin(2 * 44100);
}
void loop() {
  readSensor();
}
void readSensor( ) {

  vibVal = digitalRead(vibPin);
  micVal = analogRead(micPin);
  total = total - readings[readIndex];
  readings[readIndex] = micVal;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  average = total / numReadings;

  Serial.print(micVal);
  Serial.print(",");
  Serial.println(vibVal);
  if (micVal > (average + 150) && vibVal == 0) {
    digitalWrite(12, HIGH);
    int count = 0;
    // open wave file from sdcard
    File myFile = SD.open("wave.wav");
    AudioZero.play(myFile);

    digitalWrite(12, LOW);

  }
  else {
    digitalWrite(12, LOW);

  }
}
