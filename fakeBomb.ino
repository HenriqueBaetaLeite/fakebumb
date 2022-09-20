#include <VarSpeedServo.h>

VarSpeedServo servo;

const int ledRedBomb = 2;
const int ledYellowBomb = 3;
const int ledGreenBomb = 4;


bool redLedState = false;
bool greenLedState = false;
bool yellowLedState = false;

bool allLedsState = false;

bool gameNotStarted = true;
bool functionStartActivated = false;

const int disarmBombButton = 5;
const int startOpenBombButton = 6;

const int buzzer = 7;

const int myServo = 8;

const int ledInside = 9;

unsigned long lastTime1 = 0;
unsigned long lastTime2 = 0;
unsigned long lastTime3 = 0;

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

void waitToStartGame()
{
  while (gameNotStarted)
  {
    blinkLeds();

    bool isButtonPressed = !digitalRead(startOpenBombButton);

    if (isButtonPressed == HIGH)
    {
      functionStartActivated = true;
      gameNotStarted = false;
    }
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Entrei no setup");
  Serial.println(millis());

  pinMode(disarmBombButton, INPUT_PULLUP);
  pinMode(startOpenBombButton, INPUT_PULLUP);

  pinMode(ledRedBomb, OUTPUT);
  pinMode(ledYellowBomb, OUTPUT);
  pinMode(ledGreenBomb, OUTPUT);
  pinMode(ledInside, OUTPUT);

  servo.attach(myServo);

  // servo.slowmove(100, 20);
}

// LOOP ARDUINO

void loop()
{
  waitToStartGame();
  while (millis() < totalTime)
  {

    ledTimerSync();

    if (digitalRead(startOpenBombButton) == LOW && functionStartActivated)
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
  servo.slowmove(100, 20);
  resetFunc();
}