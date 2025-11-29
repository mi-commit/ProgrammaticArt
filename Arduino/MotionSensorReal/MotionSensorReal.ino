/*
  Arduino Starter Kit example
  Project 13 - Touch Sensor Lamp

  This sketch is written to accompany Project 13 in the Arduino Starter Kit

  Parts required:
  - 1 megohm resistor
  - metal foil or copper mesh
  - 220 ohm resistor
  - LED

  Software required :
  - CapacitiveSensor library by Paul Badger
    https://www.arduino.cc/reference/en/libraries/capacitivesensor/

  created 18 Sep 2012
  by Scott Fitzgerald

  https://store.arduino.cc/genuino-starter-kit

  This example code is part of the public domain.
*/

// import the library (must be located in the Arduino/libraries directory)
#include <CapacitiveSensor.h>

// create an instance of the library
// pin 4 sends electrical energy
// pin 2 senses senses a change
CapacitiveSensor capSensor = CapacitiveSensor(4, 2);

// threshold for turning the lamp on
int threshold = 130;

// pin the LED is connected to
const int ledPin = 12;

const int SAMPLECOUNT = 20;
int CurrentSample = 0;
long Samples[SAMPLECOUNT];


void setup() {
  // open a serial connection
  Serial.begin(9600);
  // set the LED pin as an output
  pinMode(ledPin, OUTPUT);
}

void loop() {
    delay(10);

  // store the value reported by the sensor in a variable
  long sensorValue = capSensor.capacitiveSensor(30);
  AddToAverage(sensorValue);
  long average = Average();
  // print out the sensor value
  Serial.print(sensorValue);
  Serial.print(",");
  Serial.println(average);

  // if the value is greater than the threshold
  if (average > threshold) {
    // turn the LED on
    digitalWrite(ledPin, HIGH);
  }
  // if it's lower than the threshold
  else {
    // turn the LED off
    digitalWrite(ledPin, LOW);
  }
}
void AddToAverage(long value){
  if(CurrentSample >= SAMPLECOUNT){
    CurrentSample = 0;
  }
  Samples[CurrentSample] = value;
  CurrentSample++;


}
long Average(){
  long rollingAverage = 0;
  for(int i = 0; i< SAMPLECOUNT; i++){
    rollingAverage += Samples[i];
  }
  return rollingAverage/SAMPLECOUNT;
}
