#ifndef WIFI_DRIVER_H
#define WIFI_DRIVER_H

#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"

class WifiDriver
{
private:
    static inline bool connected = false; // status koneksi
    void wifi_event_handler(esp_event_base_t event_base, int32_t event_id, void *event_data);

public:
    WifiDriver();
    void initWifiDriver(const char *ssid, const char *password);
    bool isconnectedWifi();
    // void reconnectWifi();
    ~WifiDriver();
    static void wifi_event_handler_static(void *arg, esp_event_base_t event_base,
                                         int32_t event_id, void *event_data);
};

#endif