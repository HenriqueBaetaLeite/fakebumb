#include <VarSpeedServo.h>

VarSpeedServo servo;

extern volatile unsigned long timer0_millis = 0;

const int ledRedBomb = 2;
const int ledYellowBomb = 3;
const int ledGreenBomb = 4;

bool redLedState = false;
bool greenLedState = false;
bool yellowLedState = false;

bool allLedsState = false;

bool gameNotStarted = true;

const int disarmBombButton = 5;
const int startOpenBombButton = 6;

const int buzzer = 7;

const int myServo = 8;

const int ledInside = 9;

unsigned long timeButtonPressed = 0;

unsigned long lastTime1 = 0;
unsigned long lastTime2 = 0;
unsigned long lastTime3 = 0;

// Must press the button for 3seconds to start the game
unsigned long timeForStartGame = 3000;

unsigned long lastTimeBlinkLeds = 0;

unsigned long oneSecondInterval = 1000;

// Fix time here, its only 2 seconds, not 2 minutes
unsigned long twoMinutesInterval = 2000;
unsigned long fourMinutesInterval = 4000;
unsigned long sixMinutesInterval = 6000;

unsigned long totalTime = 10000;

bool itIsOpen = false;

void (*resetFunc)(void) = 0;

void blinkLeds()
{
  unsigned long actualTime = millis();
  if (actualTime - lastTimeBlinkLeds >= oneSecondInterval)
  {
    allLedsState = !allLedsState;
    digitalWrite(ledGreenBomb, allLedsState);
    digitalWrite(ledRedBomb, allLedsState);
    digitalWrite(ledYellowBomb, allLedsState);
    lastTimeBlinkLeds = millis();
  }
}

void ledTimerSync()
{
  unsigned long actualTime = millis();
  if (actualTime - lastTime1 >= twoMinutesInterval)
  {
    digitalWrite(ledGreenBomb, greenLedState);
    lastTime1 = actualTime;
  }

  if (actualTime - lastTime2 >= fourMinutesInterval)
  {
    digitalWrite(ledYellowBomb, yellowLedState);
    lastTime2 = actualTime;
  }

  if (actualTime - lastTime3 >= sixMinutesInterval)
  {
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
  pinMode(ledInside, OUTPUT);

  servo.attach(myServo);

  servo.slowmove(100, 20);

  while (gameNotStarted)
  {
    blinkLeds();

    bool isButtonPressed = digitalRead(startOpenBombButton);

    if (isButtonPressed)
    {
      timeButtonPressed = millis();
      while (millis() - timeButtonPressed < timeForStartGame && isButtonPressed)
      {
      }

      if (millis() - timeButtonPressed >= timeForStartGame)
      {
        // noInterrupts();
        // timer0_millis = 0;
        // interrupts();
        gameNotStarted = false;
      }
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
    }

    if (digitalRead(disarmBombButton) == LOW)
    {
      digitalWrite(ledInside, HIGH);
    }

    if (itIsOpen)
    {
      servo.slowmove(0, 20);
    }

    if (!itIsOpen)
    {
      servo.slowmove(100, 20);
    }
  }
  resetFunc();
}