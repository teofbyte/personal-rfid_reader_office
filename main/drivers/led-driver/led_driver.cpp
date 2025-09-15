#include "led_driver.h"

LedDriver::LedDriver(gpio_num_t gpio) : pin(gpio)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << pin);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    offLed();
}

void LedDriver::onLed()
{
    gpio_set_level(pin, 0);
    ledState = true;
}

void LedDriver::offLed()
{
    gpio_set_level(pin, 1);
    ledState = false;
}

void LedDriver::blinkLed(int delay_ms)
{
    // konversi ms → ticks
    if (delayTicks == 0)
        delayTicks = pdMS_TO_TICKS(delay_ms);

    counter++;
    if (counter >= delayTicks)
    {
        // toggle LED
        if (ledState)
            offLed();
        else
            onLed();

        counter = 0; // reset counter
    }
}