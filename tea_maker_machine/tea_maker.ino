#include <TimerOne.h>
#include <Servo.h>

Servo servoMotor;

const int Data = 7;
const int Clock = 8;
const int Latch = 6;

const int SEG0 = 5;
const int SEG1 = 4;
const int SEG2 = 10;
const int SEG3 = 11;

const int setTimeButtonPin = 3;
const int startButtonPin = 2;
const int servoMotorPin = 12;
volatile int counter = 0;
volatile int flagStart = 0;
volatile int flagSet = 0;
int led = 9;
unsigned long previousMillis = 0;
const long interval = 1000;  // 1 second

int cc;
char Value[5];

const char SegData[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };

void setup() {

  Serial.begin(9600);

  pinMode(Data, OUTPUT);
  pinMode(Clock, OUTPUT);
  pinMode(Latch, OUTPUT);
  pinMode(SEG0, OUTPUT);
  pinMode(SEG1, OUTPUT);
  pinMode(SEG2, OUTPUT);
  pinMode(SEG3, OUTPUT);

   pinMode(led, OUTPUT);
  cc = 0;


  servoMotor.attach(servoMotorPin);
  servoMotor.write(130);
  delay(1000);

  pinMode(setTimeButtonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(setTimeButtonPin), setTimeFunction, RISING);

  pinMode(startButtonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(startButtonPin), startFunction, RISING);
}

void loop() {
  unsigned long currentMillis = millis();
  sprintf(Value, "%04d", counter);

  if (flagSet == 1) {
     digitalWrite(led, LOW);
    counter = 15;
    flagSet = 0;
  }


  if (flagStart == 1) {
    servoMotor.write(90);
    delay(5);
    digitalWrite(led, LOW);
  
    if (counter > 0) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        sprintf(Value, "%04d", counter);
        counter--;
      }
    }
     else {
   
      servoMotor.write(130);
      delay(5);
      digitalWrite(led, HIGH);
      flagStart = 0;
      counter = 0;
    }
  }

  
  for (cc = 0; cc < 4; cc++) {
    displayDigitOnCathode(cc);
    delay(4);  
  }
}

void DisplayDigit(char d) {
  int i;

  for (i = 0; i < 8; i++) {
    if ((d & 0x80) == 0x80) {
      digitalWrite(Data, HIGH);
    } else {
      digitalWrite(Data, LOW);
    }
    d = d << 1;

    digitalWrite(Clock, LOW);
    digitalWrite(Clock, HIGH);
  }
 
  digitalWrite(Latch, LOW);
  digitalWrite(Latch, HIGH);
}


void displayDigitOnCathode(int cathode) {


  switch (cathode) {
    case 0:
      digitalWrite(SEG0, LOW);
      DisplayDigit(SegData[Value[0] - '0']);
      digitalWrite(SEG1, HIGH);
      digitalWrite(SEG2, HIGH);
      digitalWrite(SEG3, HIGH);

      break;
    case 1:

      digitalWrite(SEG1, LOW);
      DisplayDigit(SegData[Value[1] - '0']);
      digitalWrite(SEG2, HIGH);
      digitalWrite(SEG3, HIGH);
      digitalWrite(SEG0, HIGH);
      break;
    case 2:
      digitalWrite(SEG2, LOW);
      DisplayDigit(SegData[Value[2] - '0']);
      digitalWrite(SEG1, HIGH);
      digitalWrite(SEG0, HIGH);
      digitalWrite(SEG3, HIGH);
      break;
    case 3:
      digitalWrite(SEG3, LOW);
      DisplayDigit(SegData[Value[3] - '0']);
      digitalWrite(SEG1, HIGH);
      digitalWrite(SEG2, HIGH);
      digitalWrite(SEG0, HIGH);
      break;
  }
}

void setTimeFunction() {
  flagSet = 1;
}

void startFunction() {

  flagStart = 1;
 
}