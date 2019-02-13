#define INPUT_PIN 0
#define OUTPUT_PIN 1
#define ON_DELAY 1000
#define OFF_DELAY 2000

unsigned long lastTimeOn = 0;
unsigned long scheduledOnTime = 0;
boolean scheduled  = false;

void setup() {
    pinMode(INPUT_PIN, INPUT_PULLUP);
    pinMode(OUTPUT_PIN, OUTPUT);
    digitalWrite(OUTPUT_PIN, HIGH);

    // This is a workaround for the long cpu boot up time (which takes ~5 seconds): if the INPUT_PIN is already low when booting up, then the output should be turned on immediately without any additional delay.
    if (digitalRead(INPUT_PIN) == LOW) {
        scheduled = true;
        scheduledOnTime = millis();
    }
}

void loop() {
    int currentState = digitalRead(INPUT_PIN);
    if (currentState == LOW) {
        if (!scheduled) {
            scheduledOnTime = millis() + ON_DELAY;
            scheduled = true;
        } else if (scheduled && scheduledOnTime <= millis()) {
            digitalWrite(OUTPUT_PIN, LOW);
            lastTimeOn = millis();
        }
    } else if (millis() - lastTimeOn > OFF_DELAY) {
        digitalWrite(OUTPUT_PIN, HIGH);
        scheduled = false;
    }
    delay(100);
}
