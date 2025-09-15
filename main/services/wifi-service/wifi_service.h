#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include "drivers/led-driver/led_driver.h"
#include "drivers/wifi-driver/wifi_driver.h"
#include "freertos/event_groups.h"
#include "esp_event.h"

class WifiService
{
private:
    WifiDriver wifi_driver;
    LedDriver led_driver;

public:
    WifiService(WifiDriver& wifi, LedDriver& led);
    void wifiIndicatorStatus();
};

#endif