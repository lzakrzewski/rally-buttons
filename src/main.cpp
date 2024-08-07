#include <Arduino.h>
#include <Bounce2.h>

Bounce2::Button button = Bounce2::Button();

#define LED_PIN 7
#define BUTTON_PIN 4
#define BOUNCE_INTERVAL_IN_MS 5

void setup() {
    Serial.begin(921600);

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    button.attach( BUTTON_PIN, INPUT_PULLUP );
    button.interval(BOUNCE_INTERVAL_IN_MS);
    button.setPressedState(LOW);

    digitalWrite(LED_PIN,LOW);

}

void loop() {
    // UPDATE THE BUTTON
    // YOU MUST CALL THIS EVERY LOOP
    button.update();

    if (button.fell()) {
        Serial.println("test. ");
    }

    if ( button.isPressed() ) {

        digitalWrite(LED_PIN,HIGH);


    } else {
        digitalWrite(LED_PIN,LOW);
    }
}