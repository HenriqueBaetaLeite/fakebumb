const int disarmButton = 4;

const int ledRedBomb1 = 2;

const int buzzer = 3;

// Total time of 6 minutes
long totaTime = 360000;

unsigned long lastTime = 0;
unsigned long twoMinutesInterval = 120000;

void setup() {
    Serial.begin(9600);
    pinMode(ledRedBomb1, OUTPUT);
}

void loop() {
    if (millis() >= lastTime + twoMinutesInterval) {
        Serial.println(millis());
        lastTime = millis();
        digitalWrite(ledRedBomb1, HIGH);
    }
    
    digitalWrite(ledRedBomb1, LOW);
}
