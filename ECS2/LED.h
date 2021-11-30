#ifndef ECS2_LED_H
#define ECS2_LED_H

#include <Arduino.h>

class LED {
private:
    byte ledPin;
public:
    LED(byte PIN);
    void init(String type);
    void on();
    void off();
};

#endif //ECS2_LED_H
