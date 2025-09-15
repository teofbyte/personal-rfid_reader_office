#include "wifi_service.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "nvs_flash.h"

static const char *TAG = "WifiService";

WifiService::WifiService(WifiDriver &wifi, LedDriver &led)
    : wifi_driver(wifi), led_driver(led) {}

void WifiService::wifiIndicatorStatus()
{
    if (wifi_driver.isconnectedWifi())
    {
        ESP_LOGI(TAG, "WiFi Connected");
        led_driver.onLed();
    }
    else
    {
        ESP_LOGW(TAG, "WiFi Disconnected");
        led_driver.blinkLed(300); // LED kedip pelan saat belum connect
    }
}