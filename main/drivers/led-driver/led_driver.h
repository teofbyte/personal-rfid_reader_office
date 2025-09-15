#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class LedDriver
{
private:
    gpio_num_t pin;
    bool ledState;
    int counter;
    int delayTicks;

public:
    LedDriver(gpio_num_t gpio);
    void onLed();
    void offLed();
    void blinkLed(int delay_ms);
};

#endif