#include <Arduino.h>
#include <Bounce2.h>
#include <BleKeyboard.h>
#include <list>
#include <WiFi.h>
#include <esp_sleep.h>
#include <Keypad.h>
#include <map>
#include <unordered_map>
#include <utility>
#include <lwip/pbuf.h>

#define BOUNCE_INTERVAL_IN_MS 100
#define LONG_PRESS_TIME 500
#define DEBUG 1
#define DEFAULT_MAP "enduro_rally"
#define SECONDARY_MAP "locus"

void debug(const std::string &message) {
    if (DEBUG) {
        Serial.println(message.c_str());
    }
}

struct ButtonActivity {
    std::string button;
    unsigned long occuredAt;

    ButtonActivity(const std::string &button, unsigned long occuredAt) {
        this->button = button;
        this->occuredAt = occuredAt;
    }
};

class Button {
public:
    char id;
    std::string name;
    std::function<void()> primaryAction;
    bool isMapSelector;
    std::string map;

    Button(
        const char id,
        const std::string &name,
        const std::function<void()> &action,
        const bool isMapSelector,
        const std::string &map
    ) {
        this->id = id;
        this->name = name;
        this->primaryAction = action;
        this->isMapSelector = isMapSelector;
        this->map = map;
    }
};

class ButtonTracker {
    std::unordered_map<std::string, ButtonActivity> buttonPressedActivities;
    std::unordered_map<std::string, ButtonActivity> buttonReleasedActivities;

public:
    ButtonTracker() {
    }

    void trackButtonPressed(const Button &button) {
        this->buttonPressedActivities.emplace(button.name, ButtonActivity(button.name, millis()));
    }

    void trackButtonReleased(const Button &button) {
        this->buttonReleasedActivities.emplace(button.name, ButtonActivity(button.name, millis()));
    }

    bool hasBeenPressedFor(const Button &button, int8_t seconds) {
        unsigned long diff = this->buttonReleasedActivities.at(button.name).occuredAt
                             - this->buttonPressedActivities.at(button.name).occuredAt;

        debug("Diff:  " + std::to_string(diff));

        if ((diff / 1000) >= seconds) {
            return true;
        }

        return false;
    }
};

class Buttons {
    std::map<char, Button> buttons;
    ButtonTracker tracker;
    bool isSelectingMap;
    std::string currentMap;

    Button getButtonById(const char id) const {
        return this->buttons.at(id);
    }

public:
    Buttons(const std::list<Button> &buttons, const ButtonTracker &tracker): isSelectingMap(false),
                                                                             currentMap(DEFAULT_MAP) {
        for (const auto &button: buttons) {
            this->buttons.emplace(button.id, button);
        }

        this->tracker = tracker;
    }

    void handle(KeypadEvent key, Keypad keypad) {
        const Button currentButton = this->getButtonById(key);

        switch (keypad.getState()) {
            case PRESSED:
                debug("Button " + currentButton.name + " id pressed.");

                this->tracker.trackButtonPressed(currentButton);

                if (this->isSelectingMap) {
                    this->isSelectingMap = false;

                    debug("Map " + currentButton.map + "selected");

                } else {
                    currentButton.primaryAction();
                }

                break;
            case IDLE:
                debug("Button " + currentButton.name + " is idling.");
                break;
            case HOLD:
                debug("Button " + currentButton.name + " is held.");

                break;
            case RELEASED:
                debug("Button " + currentButton.name + " is released.");

                this->tracker.trackButtonReleased(currentButton);

                if (currentButton.isMapSelector && this->tracker.hasBeenPressedFor(currentButton, 10)) {
                    this->isSelectingMap = true;

                    debug("Currently selecting map...");
                }

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

BleKeyboard bleKeyboard("rally buttons", "behind.bushes electronics");

Buttons buttons = Buttons(
    {
        Button(
            '1',
            "1red-r",
            []() {
                bleKeyboard.write(KEY_UP_ARROW);
            },
            false,
            DEFAULT_MAP
        ),
        Button(
            '2',
            "2yellof-f",
            []() {
                bleKeyboard.write(KEY_UP_ARROW);
                bleKeyboard.write(KEY_UP_ARROW);
            },
            false,
            DEFAULT_MAP
        ),
        Button(
            '3',
            "3yellof-r", []() {
                bleKeyboard.write(KEY_DOWN_ARROW);
            },
            false,
            DEFAULT_MAP
        ),
        Button(
            '4',
            "4red-f", []() {
                bleKeyboard.write(KEY_LEFT_ARROW);
                bleKeyboard.write(KEY_LEFT_ARROW);
            },
            false,
            DEFAULT_MAP
        ),
        Button(
            '5',
            "5blue-f", []() {
                bleKeyboard.write(KEY_DOWN_ARROW);
                bleKeyboard.write(KEY_DOWN_ARROW);
            },
            false,
            DEFAULT_MAP
        ),
        Button(
            '6',
            "6grey-f", []() {
                bleKeyboard.write(KEY_RIGHT_ARROW);
                bleKeyboard.write(KEY_RIGHT_ARROW);
            },
            false,
            SECONDARY_MAP
        ),
        Button(
            '7',
            "7green-f", []() {
                bleKeyboard.write(KEY_LEFT_ARROW);
                bleKeyboard.write(KEY_LEFT_ARROW);
            },
            true,
            DEFAULT_MAP
        ),
        Button(
            '8',
            "8unknown", []() {
            },
            false,
            DEFAULT_MAP
        ),
    },
    ButtonTracker()
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
