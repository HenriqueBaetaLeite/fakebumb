#include <VarSpeedServo.h> 

VarSpeedServo servo;

const int ledRedBomb = 2;
const int ledYellowBomb = 3;
const int ledGreenBomb = 4;

const int disarmBombButton = 5;
const int openBombButton = 6;

const int buzzer = 7;

const int myServo = 8;

const int ledInside = 9;

unsigned long lastTime = 0;
unsigned long lastTime2 = 0;
unsigned long lastTime3 = 0;

unsigned long twoMinutesInterval = 2000;
unsigned long fourMinutesInterval = 4000;
unsigned long sixMinutesInterval = 6000;

unsigned long totalTime = 10000;

bool itIsOpen = false;

void(* resetFunc) (void) = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Entrei no setup");

  pinMode(disarmBombButton, INPUT_PULLUP);
  pinMode(openBombButton, INPUT_PULLUP);

  pinMode(ledRedBomb, OUTPUT);
  pinMode(ledYellowBomb, OUTPUT);
  pinMode(ledGreenBomb, OUTPUT);
  digitalWrite(ledRedBomb, HIGH);
  digitalWrite(ledYellowBomb, HIGH);
  digitalWrite(ledGreenBomb, HIGH);

  pinMode(ledInside, OUTPUT);

  servo.attach(myServo);
}
// LOOP ARDUINO

void loop()
{  
  while (millis() < totalTime) {

    if (digitalRead(openBombButton) == LOW) {
      delay(50);
      itIsOpen = true;
      Serial.println("Yessss!");
    }

    if(digitalRead(disarmBombButton) == LOW) {
      delay(10);
      digitalWrite(ledInside, HIGH);
    }

    if (itIsOpen) {
       servo.slowmove(0, 20);
       Serial.println("it is open!" + itIsOpen);
    }
    
    if (!itIsOpen) {
      servo.slowmove(100, 20);
      Serial.println("it is close!" + itIsOpen);
    }
    
    if (millis() - lastTime >= twoMinutesInterval) {
      digitalWrite(ledRedBomb, LOW);
      lastTime = millis();
    }

    if (millis() - lastTime2 >= fourMinutesInterval) {
      digitalWrite(ledYellowBomb, LOW);
      lastTime2 = millis();
    }

    if (millis() - lastTime3 >= sixMinutesInterval) {
      digitalWrite(ledGreenBomb, LOW);
      lastTime3 = millis();
    }
  }
  resetFunc();  
}