#include "LED.h"

LED::LED(byte PIN) {
    ledPin = PIN;
}

void LED::init(String type) {
    if (type == "Output") {
        pinMode(ledPin, OUTPUT);
        off();
    }
    else if (type == "Input") {
        pinMode(ledPin, INPUT);
        off();
    }
}

void LED::on() {
    digitalWrite(ledPin, HIGH);
}

void LED::off() {
    digitalWrite(ledPin, LOW);
}
