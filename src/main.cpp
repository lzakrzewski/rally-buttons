#include <Arduino.h>
#include <Bounce2.h>
#include <BleKeyboard.h>
#include <list>
#include <WiFi.h>
#include <esp_sleep.h>
#include <Keypad.h>
#include <map>
#include <lwip/pbuf.h>

#define BOUNCE_INTERVAL_IN_MS 100
#define LONG_PRESS_TIME 500
#define DEBUG 1
#define DEFAULT_PROFILE "enduro_rally"

void debug(const std::string &message) {
    if (DEBUG) {
        Serial.println(message.c_str());
    }
}

class Button {
public:
    char id;
    std::string name;
    std::function<void()> primaryAction;

    Button(const char id, const std::string &name, const std::function<void()> &action) {
        this->id = id;
        this->name = name;
        this->primaryAction = action;
    }
};

class ProfileSwitch {
private:
    std::string defaultProfile;

public:
    nonstd::optional<Button> currentlyPressedButton;
    nonstd::optional<int> currentlyPressedButtonAt;
    nonstd::optional<int> currentlyPressedButtonDuration;

    nonstd::optional<Button> previouslyPressedButton;
    nonstd::optional<int> previouslyPressedButtonDuration;

    ProfileSwitch() {
        this->defaultProfile = DEFAULT_PROFILE;
    }

    void trackButtonPressed(Button button) {
        this->previouslyPressedButton = this->currentlyPressedButton;
        this->previouslyPressedButtonDuration = this->previouslyPressedButtonDuration;

        this->currentlyPressedButton = nonstd::optional<Button>(button);
        this->currentlyPressedButtonAt = nonstd::optional<int>(millis());
    }

    void trackButtonReleased() {
        this->currentlyPressedButtonDuration = millis() - this->currentlyPressedButtonAt.value();
    }

    void handleProfileSwitch() {
        debug("Current duration:  type: " + (int) this->currentlyPressedButtonDuration.value());


        if (!this->currentlyPressedButton.has_value()) {
            debug("No need to switch profile, button not yet pressed");
            return;
        }



        if (!this->previouslyPressedButtonDuration.has_value()) {
            debug("No need to switch profile, previous button not yet pressed");
            return;
        }

        debug("Previous duration: " + this->previouslyPressedButtonDuration.value());
    }
};

class Buttons {
private:
    std::map<char, Button> buttons;
    ProfileSwitch profileSwitch;

    Button getButtonById(const char id) const {
        return this->buttons.at(id);
    }

public:
    Buttons(const std::list<Button> &buttons, const ProfileSwitch &profileSwitch): profileSwitch() {
        for (const auto &button: buttons) {
            this->buttons.emplace(button.id, button);
        }

        this->profileSwitch = profileSwitch;
    }

    void handle(KeypadEvent key, Keypad keypad) {
        const Button currentButton = this->getButtonById(key);

        switch (keypad.getState()) {
            case PRESSED:
                debug("Button " + currentButton.name + " id pressed.");

                this->profileSwitch.trackButtonPressed(currentButton);
                currentButton.primaryAction();
                break;
            case IDLE:
                debug("Button " + currentButton.name + " is idling.");
                break;
            case HOLD:
                debug("Button " + currentButton.name + " is held.");

                break;
            case RELEASED:
                debug("Button " + currentButton.name + " is released.");

                this->profileSwitch.trackButtonReleased();
                this->profileSwitch.handleProfileSwitch();

                break;
        }
    }
};

const byte COLS = 4;
const byte ROWS = 2;

byte colPins[COLS] = {1, 0, 4, 5};
byte rowPins[ROWS] = {2, 3};

char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', '4'},
    {'5', '6', '7', '8'},
};

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

BleKeyboard bleKeyboard("rally buttons", "Tukasz electronics");

Buttons buttons = Buttons(
    {
        Button('1', "1red-r", []() {
            bleKeyboard.write(KEY_UP_ARROW);
        }),
        Button('2', "2yellof-f", []() {
            bleKeyboard.write(KEY_UP_ARROW);
            bleKeyboard.write(KEY_UP_ARROW);
        }),
        Button('3', "3yellof-r", []() {
            bleKeyboard.write(KEY_DOWN_ARROW);
        }),
        Button('4', "4red-f", []() {
            bleKeyboard.write(KEY_LEFT_ARROW);
            bleKeyboard.write(KEY_LEFT_ARROW);
        }),
        Button('5', "5blue-f", []() {
            bleKeyboard.write(KEY_DOWN_ARROW);
            bleKeyboard.write(KEY_DOWN_ARROW);
        }),
        Button('6', "6grey-f", []() {
            bleKeyboard.write(KEY_RIGHT_ARROW);
            bleKeyboard.write(KEY_RIGHT_ARROW);
        }),
        Button('7', "7green-f", []() {
            bleKeyboard.write(KEY_LEFT_ARROW);
            bleKeyboard.write(KEY_LEFT_ARROW);
        }),
        Button('8', "8unknown", []() {
        }),
    },
    ProfileSwitch()
);

void setup() {
    Serial.begin(115200);
    keypad.setDebounceTime(BOUNCE_INTERVAL_IN_MS);
    keypad.setHoldTime(LONG_PRESS_TIME);
    keypad.addEventListener([](KeypadEvent key) { buttons.handle(key, keypad); });

    bleKeyboard.begin();

    debug("Setup complete.");
}

void loop() {
    keypad.getKey();

    if (bleKeyboard.isConnected()) {
        return;
    }

    delay(10);
}
