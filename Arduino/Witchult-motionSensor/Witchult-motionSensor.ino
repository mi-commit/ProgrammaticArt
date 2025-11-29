const byte pirPin = 10;
const byte ledPin = 13; // onboard LED
const byte buzzPin = 8;

void setup() {
  pinMode(pirPin,INPUT_PULLUP);
  pinMode(ledPin,OUTPUT);
  pinMode(buzzPin, OUTPUT);
  Serial.begin(9600);
}

int lastState;

void loop() {
  int state = digitalRead(pirPin);
  digitalWrite(ledPin, state); // LED follows PIR output
  if(state == HIGH && lastState != state){
    Serial.println("Movement");
    tone(buzzPin, 100, 50);
  }
  lastState = state;

  delay(20);
}