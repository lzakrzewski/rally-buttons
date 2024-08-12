#include <Arduino.h>
#include <Bounce2.h>
#include <BleKeyboard.h>
#include <list>

#define LED_PIN 2
#define LED_PIN2 7
#define BUTTON_PIN 4
#define BUTTON_PIN2 3
#define BOUNCE_INTERVAL_IN_MS 100

class Button {
public:
    int id;
    int pin;
    int led;
    int action;

    Bounce2::Button bounce;
    BleKeyboard bleKeyboard;

    Button(const int id, const int pin, const int led, const int action) {
        this->id = id;
        this->pin = pin;
        this->led = led;
        this->action = action;
        this->bounce = Bounce2::Button();
    }

    void init(const BleKeyboard &bleKeyboard) {
        pinMode(this->pin, INPUT_PULLUP);

        this->bounce.attach(this->pin, INPUT_PULLUP);
        this->bounce.interval(BOUNCE_INTERVAL_IN_MS);
        this->bounce.setPressedState(LOW);

        this->bleKeyboard = bleKeyboard;
    }

    void update() {
        this->bounce.update();

        if (this->bounce.fell()) {
            Serial.println("Button . " + static_cast<String>(this->id) + " pressed.");

            if(bleKeyboard.isConnected()) {
                bleKeyboard.write(this->action);
            } else {
                Serial.println("Unable to init BT.");
            }
        }

        if (this->bounce.isPressed()) {
            digitalWrite(this->led,HIGH);
        } else {
            digitalWrite(this->led,LOW);
        }
    }
};

BleKeyboard bleKeyboard("rally buttons", "Tukasz electronics");

std::list<Button> buttons = {
    Button(1, 3, LED_PIN, KEY_UP_ARROW),
    Button(2, 4, LED_PIN, KEY_DOWN_ARROW),
};

void setup() {
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);

    Serial.println("Setup complete.");

    bleKeyboard.begin();

    for (Button &button: buttons) {
        button.init(bleKeyboard);
    }
}

void loop() {
    for (Button &button: buttons) {
        button.update();
    }
}
