#include <VarSpeedServo.h>

VarSpeedServo servo;

const int ledRedBomb = 2;
const int ledYellowBomb = 3;
const int ledGreenBomb = 4;

bool redLedState = false;
bool greenLedState = false;
bool yellowLedState = false;

bool gameNotStarted = true;

const int disarmBombButton = 5;
const int startOpenBombButton = 6;

const int buzzer = 7;

const int myServo = 8;

const int ledInside = 9;

unsigned long timeButtonPressed = 0;

unsigned long lastTime = 0;
unsigned long lastTime2 = 0;
unsigned long lastTime3 = 0;

unsigned long lastTimeBlinkLeds = 0;

unsigned long twoMinutesInterval = 2000;
unsigned long fourMinutesInterval = 4000;
unsigned long sixMinutesInterval = 6000;

unsigned long totalTime = 10000;

bool itIsOpen = false;

void (*resetFunc)(void) = 0;

void blinkLeds() {
  unsigned long actualTime = millis();
  if (actualTime - lastTimeBlinkLeds >= 1500) {
    digitalWrite(ledGreenBomb, HIGH);
    digitalWrite(ledRedBomb, HIGH);
    digitalWrite(ledYellowBomb, HIGH);
    lastTimeBlinkLeds = millis();
  }
  else {
    digitalWrite(ledGreenBomb, LOW);
    digitalWrite(ledRedBomb, LOW);
    digitalWrite(ledYellowBomb, LOW);
    // lastTimeBlinkLeds = millis();
  }
}

void ledTimerSync()
{
  unsigned long actualTime = millis();
  if (actualTime - lastTime >= twoMinutesInterval)
  {
    // greenLedState = !greenLedState;
    digitalWrite(ledGreenBomb, greenLedState);
    lastTime = actualTime;
  }

  if (actualTime - lastTime2 >= fourMinutesInterval)
  {
    // yellowLedState = !yellowLedState;
    digitalWrite(ledYellowBomb, yellowLedState);
    lastTime2 = actualTime;
  }

  if (actualTime - lastTime3 >= sixMinutesInterval)
  {
    // redLedState = !redLedState;
    digitalWrite(ledRedBomb, redLedState);
    lastTime3 = actualTime;
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Entrei no setup");

  pinMode(disarmBombButton, INPUT_PULLUP);
  pinMode(startOpenBombButton, INPUT_PULLUP);

  pinMode(ledRedBomb, OUTPUT);
  pinMode(ledYellowBomb, OUTPUT);
  pinMode(ledGreenBomb, OUTPUT);
  digitalWrite(ledRedBomb, HIGH);
  digitalWrite(ledYellowBomb, HIGH);
  digitalWrite(ledGreenBomb, HIGH);

  pinMode(ledInside, OUTPUT);

  servo.attach(myServo);

   servo.slowmove(100, 20);

  while(gameNotStarted) {
    Serial.println("Será q deu certo???");
    blinkLeds();
    if(digitalRead(startOpenBombButton) == LOW) {
      Serial.prinln("Button pressed...");
      gameNotStarted = false;
    }
  }
}
// LOOP ARDUINO

void loop()
{
  while (millis() < totalTime)
  {

    ledTimerSync();

    if (digitalRead(startOpenBombButton) == LOW)
    {
      itIsOpen = true;
      Serial.println("Yessss!");
    }

    if (digitalRead(disarmBombButton) == LOW)
    {
      digitalWrite(ledInside, HIGH);
    }

    if (itIsOpen)
    {
      servo.slowmove(0, 20);
      Serial.println("it is open!" + itIsOpen);
    }

    if (!itIsOpen)
    {
      servo.slowmove(100, 20);
      Serial.println("it is close!" + itIsOpen);
    }
  }
  resetFunc();
}